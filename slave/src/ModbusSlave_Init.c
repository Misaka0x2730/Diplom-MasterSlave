#include "ModbusSlave_Init.h"

Modbus_Slave_State slaveState;

void ModbusSlave_State_Init()
{
	slaveState.state 			= MODBUS_RTU_RECEIVE;
	slaveState.rxData.length 	= 0;
	slaveState.txData.length 	= 0;
	slaveState.rxDelay 			= 0;
	slaveState.rxTimer 			= 0;
	slaveState.txTimer 			= 0;
	slaveState.txDelay 			= MODBUS_RTU_SEND_TIMEOUT;
	slaveState.slaveAddress 	= 0x31;
}

void ModbusSlave_Init_GPIO()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	GPIO_xInit(PIN_RS485_TXD, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);
	GPIO_xInit(PIN_RS485_RXD, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING);
	GPIO_xInit(PIN_RS485_MODE, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);

	MODBUS_RTU_SELECT_RECEIVER;
}

void ModbusSlave_Init_USART()
{
	slaveState.slaveAddress = Device_GetAddress();
	USART_DeInit(MODBUS_RTU_UART);
	MODBUS_RTU_UART_RCC_ENABLE;

#ifdef MODBUS_RTU_GPIO_REMAP
	GPIO_PinRemapConfig(MODBUS_RTU_GPIO_REMAP, ENABLE);
#endif

	USART_InitTypeDef USART_InitStructure;
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;

	switch(Device_GetBaud())
	{
		case MODBUS_RTU_BAUD_1200:
		{
			USART_InitStructure.USART_BaudRate = 1200;
			slaveState.rxDelay = MODBUS_RTU_TIMEOUT_1200_10BIT;
			break;
		}
		case MODBUS_RTU_BAUD_2400:
		{
			USART_InitStructure.USART_BaudRate = 2400;
			slaveState.rxDelay = MODBUS_RTU_TIMEOUT_2400_10BIT;
			break;
		}
		case MODBUS_RTU_BAUD_4800:
		{
			USART_InitStructure.USART_BaudRate = 4800;
			slaveState.rxDelay = MODBUS_RTU_TIMEOUT_4800_10BIT;
			break;
		}
		case MODBUS_RTU_BAUD_9600:
		{
			USART_InitStructure.USART_BaudRate = 9600;
			slaveState.rxDelay = MODBUS_RTU_TIMEOUT_9600_10BIT;
			break;
		}
		case MODBUS_RTU_BAUD_19200:
		{
			USART_InitStructure.USART_BaudRate = 19200;
			slaveState.rxDelay = MODBUS_RTU_TIMEOUT_19200_10BIT;
			break;
		}
		case MODBUS_RTU_BAUD_38400:
		{
			USART_InitStructure.USART_BaudRate = 38400;
			slaveState.rxDelay = MODBUS_RTU_TIMEOUT_38400_10BIT;
			break;
		}
		case MODBUS_RTU_BAUD_57600:
		{
			USART_InitStructure.USART_BaudRate = 57600;
			slaveState.rxDelay = MODBUS_RTU_TIMEOUT_57600_10BIT;
			break;
		}
		case MODBUS_RTU_BAUD_115200:
		{
			USART_InitStructure.USART_BaudRate = 115200;
			slaveState.rxDelay = MODBUS_RTU_TIMEOUT_115200_10BIT;
			break;
		}
		default:
		{
			USART_InitStructure.USART_BaudRate = 9600;
			slaveState.rxDelay = MODBUS_RTU_TIMEOUT_9600_10BIT;
			break;
		}
	}
	USART_Init(MODBUS_RTU_UART, &USART_InitStructure);
}

void ModbusSlave_Init_Timer()
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	u16_t waitPeriod = Device_GetWaitingTime();
	if(waitPeriod == 0)
		waitPeriod = 1;

	TIM_DeInit(MODBUS_RTU_TIM);
	MODBUS_RTU_TIM_RCC_ENABLE;
	TIM_TimeBaseStructInit(&TIM_InitStructure);
	TIM_InitStructure.TIM_Prescaler = ((u16_t)(F_APB1/1000000))*2-1;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = MODBUS_RTU_TIM_PERIOD;
	TIM_TimeBaseInit(MODBUS_RTU_TIM, &TIM_InitStructure);
	TIM_ClearITPendingBit(MODBUS_RTU_TIM, TIM_IT_Update);
	TIM_ITConfig(MODBUS_RTU_TIM,TIM_IT_Update,ENABLE);

	TIM_DeInit(MODBUS_RTU_WAIT_TIM);
	MODBUS_RTU_WAIT_TIM_RCC_ENABLE;
	TIM_TimeBaseStructInit(&TIM_InitStructure);
	TIM_InitStructure.TIM_Prescaler = ((u16_t)(F_APB1/1000)) - 1;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = waitPeriod*2 - 1;
	TIM_TimeBaseInit(MODBUS_RTU_WAIT_TIM, &TIM_InitStructure);
	TIM_ClearITPendingBit(MODBUS_RTU_WAIT_TIM, TIM_IT_Update);
	TIM_ITConfig(MODBUS_RTU_WAIT_TIM,TIM_IT_Update,ENABLE);
}

void ModbusSlave_Init_DMA()
{
	MODBUS_RTU_DMA_RCC_ENABLE;
	DMA_DeInit(MODBUS_RTU_DMA_CHANNEL);

	DMA_InitTypeDef DMA_InitStructure;

	DMA_StructInit(&DMA_InitStructure);
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32_t)&(MODBUS_RTU_UART->DR);
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_Init(MODBUS_RTU_DMA_CHANNEL, &DMA_InitStructure);
}

void ModbusSlave_Init_NVIC()
{
	NVIC_InitTypeDef  NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = MODBUS_RTU_UART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = MODBUS_RTU_TIM_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = MODBUS_RTU_WAIT_TIM_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void ModbusSlave_Start()
{
	ModbusSlave_State_Init();
	ModbusSlave_Init_GPIO();
	ModbusSlave_Init_DMA();
	ModbusSlave_Init_NVIC();
	ModbusSlave_Init_USART();
	ModbusSlave_Init_Timer();

	MODBUS_RTU_SELECT_RECEIVER;

	USART_ClearITPendingBit(MODBUS_RTU_UART, USART_IT_RXNE);
	USART_ClearITPendingBit(MODBUS_RTU_UART, USART_IT_TC);
	USART_Cmd(MODBUS_RTU_UART,ENABLE);

	TIM_ClearITPendingBit(MODBUS_RTU_TIM, TIM_IT_Update);
	TIM_Cmd(MODBUS_RTU_TIM, ENABLE);

	NVIC_EnableIRQ(MODBUS_RTU_UART_IRQn);
	NVIC_EnableIRQ(MODBUS_RTU_TIM_IRQn);
	NVIC_EnableIRQ(MODBUS_RTU_WAIT_TIM_IRQn);
}

void ModbusSlave_Stop()
{
	slaveState.state = MODBUS_RTU_RECEIVE;

	MODBUS_RTU_SELECT_RECEIVER;
	USART_Cmd(MODBUS_RTU_UART, DISABLE);
	DMA_Cmd(MODBUS_RTU_DMA_CHANNEL, DISABLE);
	TIM_Cmd(MODBUS_RTU_TIM, DISABLE);
	TIM_Cmd(MODBUS_RTU_WAIT_TIM, DISABLE);

	NVIC_DisableIRQ(MODBUS_RTU_UART_IRQn);
	NVIC_DisableIRQ(MODBUS_RTU_TIM_IRQn);
	NVIC_DisableIRQ(MODBUS_RTU_WAIT_TIM_IRQn);
}

u8_t ModbusSlave_Send(ModbusAdu* data)
{
	u16_t i = 0;

	if(data != NULL)
	{
		slaveState.state = MODBUS_RTU_TRANCEIVE;
		slaveState.txData.length = 0;
		slaveState.rxData.length = 0;

		for(i = 0; ((i < data->length) && (i < sizeof(slaveState.txData.bytes))); i++)
			slaveState.txData.bytes[slaveState.txData.length++] = data->bytes[i];

		DMA_Cmd(MODBUS_RTU_DMA_CHANNEL, DISABLE);
		DMA_SetCurrDataCounter(MODBUS_RTU_DMA_CHANNEL,slaveState.txData.length);
		MODBUS_RTU_DMA_CHANNEL->CMAR = (u32_t)(&slaveState.txData.bytes);
		MODBUS_RTU_SELECT_TRANSMITTER;

		USART_DMACmd(MODBUS_RTU_UART, USART_DMAReq_Tx, ENABLE);
		DMA_Cmd(MODBUS_RTU_DMA_CHANNEL, ENABLE);
	}
	else
	{
		slaveState.state = MODBUS_RTU_RECEIVE;
		slaveState.txData.length = 0;
		slaveState.rxData.length = 0;
		MODBUS_RTU_SELECT_RECEIVER;
	}
	return MODBUS_ERROR_OK;
}

u8_t ModbusSlave_SendResponse(ModbusPdu* pdu, u8_t errorCode)
{
	u16_t i = 0;
	u16_t crc16 = 0;

	if(pdu != NULL)
	{
		ModbusAdu adu;
		ModbusCommon_InitAdu(&adu);

		adu.bytes[0] = pdu->slaveAddress;
		if(pdu->slaveAddress == MODBUS_RTU_BROADCAST_ADDRESS)
			return ModbusSlave_Send(NULL);
		else
		{
			if((errorCode == MODBUS_ERROR_OK) && (MODBUS_IS_CMD(pdu->cmd)))
			{
				adu.bytes[1] = pdu->cmd;
				adu.length = 4;
				if((pdu->cmd == MODBUS_READ_HOLDING_REGISTERS)||(pdu->cmd == MODBUS_READ_COILS)||(pdu->cmd == MODBUS_READ_INPUT_REGISTERS)||(pdu->cmd == MODBUS_READ_DISCRETE_INPUTS))
				{
					adu.length = (1+1+1+pdu->numberOfBytes+2);//slave address+cmd+byte count+data[byte count]+crc-16
					adu.bytes[2] = pdu->numberOfBytes;
					for(i = 0; ((i < pdu->numberOfBytes) && (i < MODBUS_MAX_DATA_SIZE)); i++)
						adu.bytes[3+i] = pdu->data[i];
				}
				if((pdu->cmd == MODBUS_SET_ONE_COIL)||(pdu->cmd == MODBUS_WRITE_ONE_HOLDING_REGISTER))
				{
					adu.length = 1+1+2+2+2;//slave address+cmd+address+value+crc-16
					adu.bytes[2] = (pdu->address>>8)&0xFF;
					adu.bytes[3] = pdu->address & 0xFF;
					adu.bytes[4] = pdu->data[0];
					adu.bytes[5] = pdu->data[1];
				}
				if((pdu->cmd == MODBUS_SET_MULTIPLE_COILS)||(pdu->cmd == MODBUS_WRITE_MULTIPLE_HOLDING_REGISTERS))
				{
					adu.length = 1+1+2+2+2; //slave address+cmd+address+number of outputs+crc-16
					adu.bytes[2] = (pdu->address>>8)&0xFF;
					adu.bytes[3] = pdu->address & 0xFF;
					adu.bytes[4] = (pdu->numberOfOutputs>>8)&0xFF;
					adu.bytes[5] = pdu->numberOfOutputs & 0xFF;
				}
				if(pdu->cmd == MODBUS_READ_DEVICE_INFO)
				{
					adu.length = 1+1+1+pdu->data[0]+2;//slave address+cmd+byte count+data+crc-16
					for(i = 0; (i < (pdu->data[0] + 1)) && (i < (MODBUS_MAX_ADU_SIZE - 4)); i++)
						adu.bytes[2+i] = pdu->data[i];
				}
				if(pdu->cmd == MODBUS_LOOP_TEST)
				{
					adu.length = 1+1+1+2;//slave address+cmd+state+crc-16
					adu.bytes[2] = pdu->data[0];
				}
				if(pdu->cmd == MODBUS_READ_DIP_SWITCH_STATE)
				{
					adu.length = 1+1+2+2;//slave address+cmd+state+crc-16
					adu.bytes[2] = pdu->data[0];
					adu.bytes[3] = pdu->data[1];
				}
			}
			else
			{
				if(errorCode == MODBUS_ERROR_OK)
					errorCode = MODBUS_ILLEGAL_FUNCTION;

				adu.length = MODBUS_ERROR_ADU_LENGTH;
				adu.bytes[1] = pdu->cmd|MODBUS_ERROR_MASK;
				adu.bytes[2] = errorCode;
			}
			crc16 = ModbusCommon_CalculateCRC16(&adu);
			adu.bytes[adu.length-2] = (crc16 & 0xFF);
			adu.bytes[adu.length-1] = ((crc16>>8) & 0xFF);

			return ModbusSlave_Send(&adu);
		}
	}
	else
		return ModbusSlave_Send(NULL);
}

