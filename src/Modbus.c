#include "Modbus.h"

extern Modbus_Slave_State slaveState;
extern Modbus_Master_State masterState;

void MODBUS_IRQHandler()
{
	u8_t data = 0;
	u8_t status = 0;

	if(Device_GetMode() == MODE_SLAVE)
	{
		if (USART_GetITStatus(MODBUS_RTU_UART, USART_IT_RXNE) != RESET)
		{
			USART_ClearITPendingBit(MODBUS_RTU_UART, USART_IT_RXNE);
			if((MODBUS_RTU_UART->SR & (USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE | USART_FLAG_ORE)) == 0)
			{
				data = USART_ReceiveData(MODBUS_RTU_UART);
				if(slaveState.state == MODBUS_RTU_RECEIVE)
				{
					slaveState.rxTimer = 0;
					if(slaveState.rxData.length < sizeof(slaveState.rxData.bytes))
						slaveState.rxData.bytes[slaveState.rxData.length++] = data;
				}
			}
			else
			{
				status = MODBUS_RTU_UART->SR;
				data =  USART_ReceiveData(MODBUS_RTU_UART);
			}
		}

		if (USART_GetITStatus(MODBUS_RTU_UART, USART_IT_TC) != RESET)
		{
			USART_ClearITPendingBit(MODBUS_RTU_UART, USART_IT_TC);
			if(slaveState.state == MODBUS_RTU_TRANCEIVE)
			{
				slaveState.state = MODBUS_RTU_RECEIVE;
				slaveState.txTimer = 0;

				DMA_ClearITPendingBit(MODBUS_RTU_DMA_IT_FLAG);				//End of DMA transfer
				DMA_Cmd(MODBUS_RTU_DMA_CHANNEL, DISABLE);

				USART_DMACmd(MODBUS_RTU_UART, USART_DMAReq_Tx, DISABLE);
				USART_ITConfig(MODBUS_RTU_UART, USART_IT_TC, DISABLE);
				MODBUS_RTU_SELECT_RECEIVER;
			}
		}
	}
	else
	{
		if (USART_GetITStatus(MODBUS_RTU_UART, USART_IT_RXNE) != RESET)
		{
			USART_ClearITPendingBit(MODBUS_RTU_UART, USART_IT_RXNE);
			if((MODBUS_RTU_UART->SR & (USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE | USART_FLAG_ORE)) == 0)
			{
				data = USART_ReceiveData(MODBUS_RTU_UART);
				if(masterState.state == MODBUS_RTU_RECEIVE)
				{
					masterState.rxTimer = 0;
					if(masterState.rxData.length < sizeof(masterState.rxData.bytes))
						masterState.rxData.bytes[masterState.rxData.length++] = data;
				}
			}
			else
			{
				status = MODBUS_RTU_UART->SR;
				data =  USART_ReceiveData(MODBUS_RTU_UART);
			}
		}

		if (USART_GetITStatus(MODBUS_RTU_UART, USART_IT_TC) != RESET)
		{
			USART_ClearITPendingBit(MODBUS_RTU_UART, USART_IT_TC);
			if(masterState.state == MODBUS_RTU_TRANCEIVE)
			{
				masterState.state = MODBUS_RTU_RECEIVE;
				masterState.txTimer = 0;
				masterState.rxTimer = 0;
				masterState.txData.length = 0;
				masterState.rxData.length = 0;

				DMA_ClearITPendingBit(MODBUS_RTU_DMA_IT_FLAG);				//End of DMA transfer
				DMA_Cmd(MODBUS_RTU_DMA_CHANNEL, DISABLE);

				USART_DMACmd(MODBUS_RTU_UART, USART_DMAReq_Tx, DISABLE);
				USART_ITConfig(MODBUS_RTU_UART, USART_IT_TC, DISABLE);
				MODBUS_RTU_SELECT_RECEIVER;

				TIM_SetCounter(MODBUS_RTU_WAIT_TIM, 0);
				TIM_Cmd(MODBUS_RTU_WAIT_TIM, ENABLE);
			}
		}
	}

	if (USART_GetITStatus(MODBUS_RTU_UART, USART_IT_ORE) != RESET)
	{
		USART_ClearITPendingBit(MODBUS_RTU_UART, USART_IT_ORE);
		status = MODBUS_RTU_UART->SR;
		data =  USART_ReceiveData(MODBUS_RTU_UART);
	}

}

void MODBUS_RTU_TIM_IRQHandler()
{
	if (TIM_GetFlagStatus(MODBUS_RTU_TIM, TIM_FLAG_Update))
	{
		TIM_ClearITPendingBit(MODBUS_RTU_TIM, TIM_IT_Update);

		if(Device_GetMode() == MODE_SLAVE)
		{
			if((slaveState.state == MODBUS_RTU_RECEIVE) && (slaveState.rxData.length > 0) && (++slaveState.rxTimer > slaveState.rxDelay)) //Receive timeout
			{
				slaveState.rxTimer = 0;
				if((slaveState.rxData.bytes[0] == slaveState.slaveAddress) ||  (slaveState.rxData.bytes[0] == MODBUS_RTU_BROADCAST_ADDRESS))
				{
					USART_ITConfig(MODBUS_RTU_UART, USART_IT_RXNE, DISABLE);
					TIM_SetCounter(MODBUS_RTU_WAIT_TIM, 0);
					TIM_Cmd(MODBUS_RTU_WAIT_TIM, ENABLE);
				}
				else
					slaveState.rxData.length = 0;
			}
			if((slaveState.state == MODBUS_RTU_TRANCEIVE) && (++slaveState.txTimer > slaveState.txDelay)) //Send timeout
			{
				USART_ClearITPendingBit(MODBUS_RTU_UART, USART_IT_TC);
				USART_ITConfig(MODBUS_RTU_UART, USART_IT_TC, DISABLE);
				USART_DMACmd(MODBUS_RTU_UART, USART_DMAReq_Tx, DISABLE);

				DMA_ClearITPendingBit(MODBUS_RTU_DMA_IT_FLAG);
				DMA_Cmd(MODBUS_RTU_DMA_CHANNEL, DISABLE);

				slaveState.state = MODBUS_RTU_RECEIVE;
				slaveState.txTimer = 0;

				MODBUS_RTU_SELECT_RECEIVER;
			}
		}
		else
		{
			if((masterState.state == MODBUS_RTU_RECEIVE) && (masterState.rxData.length > 0) && (++masterState.rxTimer > masterState.rxDelay)) //Receive timeout
			{
				masterState.rxTimer = 0;
				if(masterState.rxData.bytes[0] == masterState.slaveAddress)
				{
					USART_ITConfig(MODBUS_RTU_UART, USART_IT_RXNE, DISABLE);
					ModbusMaster_Parsing(masterState.slaveAddress, &masterState.rxData);
					TIM_SetCounter(MODBUS_RTU_WAIT_TIM, 0);
					TIM_Cmd(MODBUS_RTU_WAIT_TIM, ENABLE);
				}
				else
					slaveState.rxData.length = 0;
			}
			if((slaveState.state == MODBUS_RTU_TRANCEIVE) && (++slaveState.txTimer > slaveState.txDelay)) //Send timeout
			{
				USART_ClearITPendingBit(MODBUS_RTU_UART, USART_IT_TC);
				USART_ITConfig(MODBUS_RTU_UART, USART_IT_TC, DISABLE);
				USART_DMACmd(MODBUS_RTU_UART, USART_DMAReq_Tx, DISABLE);

				DMA_ClearITPendingBit(MODBUS_RTU_DMA_IT_FLAG);
				DMA_Cmd(MODBUS_RTU_DMA_CHANNEL, DISABLE);

				slaveState.state = MODBUS_RTU_RECEIVE;
				masterState.txTimer = 0;
				masterState.rxTimer = 0;
				masterState.txData.length = 0;
				masterState.rxData.length = 0;

				MODBUS_RTU_SELECT_RECEIVER;
			}
		}
	}
	else
		MODBUS_RTU_TIM->SR = 0x0000;
}

void MODBUS_RTU_WAIT_TIM_IRQHandler()
{
	if (TIM_GetFlagStatus(MODBUS_RTU_WAIT_TIM, TIM_FLAG_Update))
	{
		TIM_ClearITPendingBit(MODBUS_RTU_WAIT_TIM, TIM_IT_Update);
		if(Device_GetMode() == MODE_SLAVE)
		{
			TIM_Cmd(MODBUS_RTU_WAIT_TIM, DISABLE);
			ModbusSlave_Parsing(slaveState.slaveAddress, &slaveState.rxData);
		}
		else
		{
			TIM_Cmd(MODBUS_RTU_WAIT_TIM, DISABLE);
			ModbusMaster_SendReadInputMessage();
		}
	}
	else
		MODBUS_RTU_WAIT_TIM->SR = 0x0000;
}
