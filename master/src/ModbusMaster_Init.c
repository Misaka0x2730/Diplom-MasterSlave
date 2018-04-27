#include "ModbusMaster_Init.h"

Modbus_Master_State masterState;

void ModbusMaster_State_Init()
{
	masterState.state 			= MODBUS_RTU_RECEIVE;
	masterState.rxData.length 	= 0;
	masterState.txData.length 	= 0;
	masterState.rxDelay 		= 0;
	masterState.rxTimer 		= 0;
	masterState.txTimer 		= 0;
	masterState.txDelay 		= MODBUS_RTU_SEND_TIMEOUT;
	masterState.slaveAddress 	= 0x31;
}

void ModbusMaster_Init_GPIO()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	GPIO_xInit(PIN_RS485_TXD, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);
	GPIO_xInit(PIN_RS485_RXD, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING);
	GPIO_xInit(PIN_RS485_MODE, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);

	MODBUS_RTU_SELECT_RECEIVER;
}

void ModbusMaster_Init_USART()
{
	masterState.slaveAddress = Device_GetAddress();
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
			masterState.rxDelay = MODBUS_RTU_TIMEOUT_1200_10BIT;
			break;
		}
		case MODBUS_RTU_BAUD_2400:
		{
			USART_InitStructure.USART_BaudRate = 2400;
			masterState.rxDelay = MODBUS_RTU_TIMEOUT_2400_10BIT;
			break;
		}
		case MODBUS_RTU_BAUD_4800:
		{
			USART_InitStructure.USART_BaudRate = 4800;
			masterState.rxDelay = MODBUS_RTU_TIMEOUT_4800_10BIT;
			break;
		}
		case MODBUS_RTU_BAUD_9600:
		{
			USART_InitStructure.USART_BaudRate = 9600;
			masterState.rxDelay = MODBUS_RTU_TIMEOUT_9600_10BIT;
			break;
		}
		case MODBUS_RTU_BAUD_19200:
		{
			USART_InitStructure.USART_BaudRate = 19200;
			masterState.rxDelay = MODBUS_RTU_TIMEOUT_19200_10BIT;
			break;
		}
		case MODBUS_RTU_BAUD_38400:
		{
			USART_InitStructure.USART_BaudRate = 38400;
			masterState.rxDelay = MODBUS_RTU_TIMEOUT_38400_10BIT;
			break;
		}
		case MODBUS_RTU_BAUD_57600:
		{
			USART_InitStructure.USART_BaudRate = 57600;
			masterState.rxDelay = MODBUS_RTU_TIMEOUT_57600_10BIT;
			break;
		}
		case MODBUS_RTU_BAUD_115200:
		{
			USART_InitStructure.USART_BaudRate = 115200;
			masterState.rxDelay = MODBUS_RTU_TIMEOUT_115200_10BIT;
			break;
		}
		default:
		{
			USART_InitStructure.USART_BaudRate = 9600;
			masterState.rxDelay = MODBUS_RTU_TIMEOUT_9600_10BIT;
			break;
		}
	}
	USART_Init(MODBUS_RTU_UART, &USART_InitStructure);
}

void ModbusMaster_Init_Timer()
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	u16_t interviewPeriod = Device_GetInterviewPeriod();
	if(interviewPeriod == 0)
		interviewPeriod = 1;

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
	TIM_InitStructure.TIM_Period = interviewPeriod*2 - 1;
	TIM_TimeBaseInit(MODBUS_RTU_WAIT_TIM, &TIM_InitStructure);
	TIM_ClearITPendingBit(MODBUS_RTU_WAIT_TIM, TIM_IT_Update);
	TIM_ITConfig(MODBUS_RTU_WAIT_TIM,TIM_IT_Update,ENABLE);
}

void ModbusMaster_Init_DMA()
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

void ModbusMaster_Init_NVIC()
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

void ModbusMaster_Start()
{
	ModbusMaster_State_Init();
	ModbusMaster_Init_GPIO();
	ModbusMaster_Init_DMA();
	ModbusMaster_Init_NVIC();
	ModbusMaster_Init_USART();
	ModbusMaster_Init_Timer();

	MODBUS_RTU_SELECT_RECEIVER;

	USART_ClearITPendingBit(MODBUS_RTU_UART, USART_IT_RXNE);
	USART_ClearITPendingBit(MODBUS_RTU_UART, USART_IT_TC);
	USART_Cmd(MODBUS_RTU_UART,ENABLE);

	TIM_ClearITPendingBit(MODBUS_RTU_TIM, TIM_IT_Update);
	TIM_Cmd(MODBUS_RTU_TIM, ENABLE);

	TIM_ClearITPendingBit(MODBUS_RTU_WAIT_TIM, TIM_IT_Update);
	TIM_Cmd(MODBUS_RTU_WAIT_TIM, ENABLE);

	NVIC_EnableIRQ(MODBUS_RTU_UART_IRQn);
	NVIC_EnableIRQ(MODBUS_RTU_TIM_IRQn);
	NVIC_EnableIRQ(MODBUS_RTU_WAIT_TIM_IRQn);
}

void ModbusMaster_Stop()
{
	masterState.state = MODBUS_RTU_RECEIVE;

	MODBUS_RTU_SELECT_RECEIVER;
	USART_Cmd(MODBUS_RTU_UART, DISABLE);
	DMA_Cmd(MODBUS_RTU_DMA_CHANNEL, DISABLE);
	TIM_Cmd(MODBUS_RTU_TIM, DISABLE);
	TIM_Cmd(MODBUS_RTU_WAIT_TIM, DISABLE);

	NVIC_DisableIRQ(MODBUS_RTU_UART_IRQn);
	NVIC_DisableIRQ(MODBUS_RTU_TIM_IRQn);
	NVIC_DisableIRQ(MODBUS_RTU_WAIT_TIM_IRQn);
}

u8_t ModbusMaster_Send(ModbusAdu* data)
{
	u16_t i = 0;

	if(data != NULL)
	{
		masterState.state = MODBUS_RTU_TRANCEIVE;
		masterState.txData.length = 0;
		masterState.rxData.length = 0;

		for(i = 0; ((i < data->length) && (i < sizeof(masterState.txData.bytes))); i++)
			masterState.txData.bytes[masterState.txData.length++] = data->bytes[i];

		DMA_Cmd(MODBUS_RTU_DMA_CHANNEL, DISABLE);
		DMA_SetCurrDataCounter(MODBUS_RTU_DMA_CHANNEL,masterState.txData.length);
		MODBUS_RTU_DMA_CHANNEL->CMAR = (u32_t)(&masterState.txData.bytes);
		MODBUS_RTU_SELECT_TRANSMITTER;

		USART_DMACmd(MODBUS_RTU_UART, USART_DMAReq_Tx, ENABLE);
		DMA_Cmd(MODBUS_RTU_DMA_CHANNEL, ENABLE);
	}
	else
	{
		masterState.state = MODBUS_RTU_RECEIVE;
		masterState.txData.length = 0;
		masterState.rxData.length = 0;
		MODBUS_RTU_SELECT_RECEIVER;



	}
	return MODBUS_ERROR_OK;
}
