#ifndef MODBUSCOMMON_INC_MODBUSCOMMON_H_
#define MODBUSCOMMON_INC_MODBUSCOMMON_H_

#include "types.h"
#include "stm32f10x_conf.h"
#include "pinMap.h"
#include "gpio.h"
#include <stddef.h>
#include <string.h>

#define MODBUS_RTU_OK							((u8_t)0)
#define MODBUS_RTU_ERROR						((u8_t)1)

#define MODBUS_RTU_SELECT_RECEIVER 				do {USART_ITConfig(MODBUS_RTU_UART, USART_IT_RXNE, ENABLE);\
													USART_ITConfig(MODBUS_RTU_UART, USART_IT_TC, DISABLE);\
													GPIO_xResetBit(PIN_RS485_MODE);} while(0)

#define MODBUS_RTU_SELECT_TRANSMITTER			do {USART_ClearITPendingBit(MODBUS_RTU_UART, USART_IT_TC); \
													USART_ITConfig(MODBUS_RTU_UART, USART_IT_TC, ENABLE); \
													GPIO_xSetBit(PIN_RS485_MODE);} while(0)

#define MODBUS_RTU_UART							USART3
#define MODBUS_RTU_UART_RCC						RCC_APB1Periph_USART3
#define MODBUS_RTU_UART_RCC_ENABLE				do{RCC_APB1PeriphClockCmd(MODBUS_RTU_UART_RCC,ENABLE);}while(0)
#define MODBUS_RTU_UART_IRQn					USART3_IRQn
#define MODBUS_IRQHandler						USART3_IRQHandler

#define MODBUS_RTU_TIM							TIM6
#define MODBUS_RTU_TIM_RCC						RCC_APB1Periph_TIM6
#define MODBUS_RTU_TIM_RCC_ENABLE 				do{RCC_APB1PeriphClockCmd(MODBUS_RTU_TIM_RCC, ENABLE);}while(0)
#define MODBUS_RTU_TIM_PERIOD					(10-1) //us
#define MODBUS_RTU_TIM_IRQn						TIM6_IRQn
#define MODBUS_RTU_TIM_IRQHandler				TIM6_IRQHandler

#define MODBUS_RTU_WAIT_TIM						TIM4
#define MODBUS_RTU_WAIT_TIM_RCC					RCC_APB1Periph_TIM4
#define MODBUS_RTU_WAIT_TIM_RCC_ENABLE 			do{RCC_APB1PeriphClockCmd(MODBUS_RTU_WAIT_TIM_RCC, ENABLE);}while(0)
#define MODBUS_RTU_WAIT_TIM_IRQn				TIM4_IRQn
#define MODBUS_RTU_WAIT_TIM_IRQHandler			TIM4_IRQHandler

#define MODBUS_RTU_DMA							DMA1
#define MODBUS_RTU_DMA_RCC						RCC_AHBPeriph_DMA1
#define MODBUS_RTU_DMA_RCC_ENABLE				do{RCC_AHBPeriphClockCmd(MODBUS_RTU_DMA_RCC, ENABLE);}while(0)
#define MODBUS_RTU_DMA_CHANNEL					DMA1_Channel2
#define MODBUS_RTU_DMA_IT_FLAG					DMA1_IT_TC2

#define MODBUS_RTU_BAUD_1200 					((u8_t)0x00)
#define MODBUS_RTU_BAUD_2400					((u8_t)0x01)
#define MODBUS_RTU_BAUD_4800					((u8_t)0x02)
#define MODBUS_RTU_BAUD_9600					((u8_t)0x03)
#define MODBUS_RTU_BAUD_19200					((u8_t)0x04)
#define MODBUS_RTU_BAUD_38400					((u8_t)0x05)
#define MODBUS_RTU_BAUD_57600					((u8_t)0x06)
#define MODBUS_RTU_BAUD_115200					((u8_t)0x07)

#define MODBUS_RTU_BROADCAST_ADDRESS 			((u8_t)0x00)
#define MODBUS_RTU_MIN_ADDRESS 					((u8_t)0x01)
#define MODBUS_RTU_MAX_ADDRESS					((u8_t)0xF7)//247

#define MODBUS_RTU_PERIOD_1200					((u16_t)(8000-1)) //in 0.5 ms
#define MODBUS_RTU_PERIOD_2400					((u16_t)(4000-1)) //example:
#define MODBUS_RTU_PERIOD_4800					((u16_t)(2000-1)) //2000 = 1s period
#define MODBUS_RTU_PERIOD_9600					((u16_t)(1000-1))
#define MODBUS_RTU_PERIOD_19200					((u16_t)(500-1))
#define MODBUS_RTU_PERIOD_38400					((u16_t)(250-1))
#define MODBUS_RTU_PERIOD_57600					((u16_t)(125-1))
#define MODBUS_RTU_PERIOD_115200				((u16_t)(62-1))
#define MODBUS_RTU_PERIOD_DEFAULT 				(MODBUS_RTU_PERIOD_9600)

#define MODBUS_RTU_TIMEOUT_1200_10BIT			((u16_t)2917)
#define MODBUS_RTU_TIMEOUT_2400_10BIT			((u16_t)1459)
#define MODBUS_RTU_TIMEOUT_4800_10BIT			((u16_t)730)
#define MODBUS_RTU_TIMEOUT_9600_10BIT			((u16_t)365)
#define MODBUS_RTU_TIMEOUT_19200_10BIT			((u16_t)182)
#define MODBUS_RTU_TIMEOUT_38400_10BIT			((u16_t)175)
#define MODBUS_RTU_TIMEOUT_57600_10BIT			((u16_t)175)
#define MODBUS_RTU_TIMEOUT_115200_10BIT			((u16_t)175)

#define MODBUS_RTU_SEND_TIMEOUT					((u32_t)300000)

#define MODBUS_RTU_ON 							((u8_t)1)
#define MODBUS_RTU_MAX_DATA_SIZE 				((u16_t)(MODBUS_RTU_ADU_MAX_SIZE - 3)) //MODBUS_RTU_ADU_MAX_SIZE - address(1 byte) - CRC(2 bytes)

#define MODBUS_RTU_RECEIVE 						((u8_t)1)
#define MODBUS_RTU_TRANCEIVE 					((u8_t)2)
#define MODBUS_RTU_PROCESSING 					((u8_t)3)

#define MODBUS_ERROR_ADU_LENGTH 				((u8_t)5)
#define MODBUS_ERROR_DATA_LENGTH 				((u8_t)2)
#define MODBUS_RTU_ADU_MAX_SIZE 				((u16_t)256)
#define MODBUS_RTU_MIN_DATA_SIZE				((u16_t)4) //slave address + cmd + crc-16
#define MODBUS_MAX_ADU_SIZE 					(MODBUS_RTU_ADU_MAX_SIZE)
#define MODBUS_MAX_DATA_SIZE 					((u16_t)253)

#define	MODBUS_READ_COILS 						((u8_t)0x01)
#define	MODBUS_READ_DISCRETE_INPUTS 			((u8_t)0x02)
#define	MODBUS_SET_MULTIPLE_COILS 				((u8_t)0x0F)
#define	MODBUS_READ_INPUT_REGISTERS 			((u8_t)0x04)
#define	MODBUS_READ_HOLDING_REGISTERS 			((u8_t)0x03)
#define MODBUS_SET_ONE_COIL						((u8_t)0x05)
#define MODBUS_WRITE_ONE_HOLDING_REGISTER 		((u8_t)0x06)
#define	MODBUS_WRITE_MULTIPLE_HOLDING_REGISTERS ((u8_t)0x10)
#define MODBUS_READ_DEVICE_INFO					((u8_t)0x11)
#define MODBUS_LOOP_TEST						((u8_t)0x46)
#define MODBUS_GET_ID							((u8_t)0x45)
#define MODBUS_READ_DIP_SWITCH_STATE			((u8_t)0x44)

#define MODBUS_IS_CMD(CMD)						((CMD == MODBUS_READ_COILS) || (CMD == MODBUS_READ_DISCRETE_INPUTS) \
												|| (CMD == MODBUS_SET_MULTIPLE_COILS) || (CMD == MODBUS_READ_INPUT_REGISTERS)\
												|| (CMD == MODBUS_READ_HOLDING_REGISTERS) || (CMD == MODBUS_SET_ONE_COIL)\
												|| (CMD == MODBUS_WRITE_ONE_HOLDING_REGISTER) || (CMD == MODBUS_WRITE_MULTIPLE_HOLDING_REGISTERS)\
												|| (CMD == MODBUS_READ_DEVICE_INFO) || (CMD == MODBUS_LOOP_TEST)\
												|| (CMD == MODBUS_GET_ID) || (CMD == MODBUS_READ_DIP_SWITCH_STATE))

#define MODBUS_ERROR_OK 						((u8_t)0x00)
#define	MODBUS_ILLEGAL_FUNCTION 				((u8_t)0x01)
#define	MODBUS_ILLEGAL_DATA_ADDRESS 			((u8_t)0x02)
#define	MODBUS_ILLEGAL_DATA_VALUE 				((u8_t)0x03)
#define MODBUS_UNDEFINED_ERROR 					((u8_t)0x04)
#define MODBUS_ERROR_CRC	   					((u8_t)0x05)
#define MODBUS_ERROR_SLAVE_ADDRESS				((u8_t)0x06)
#define MODBUS_ERROR_MASK 						((u8_t)0x80)

#define COILS_MAX_AMOUNT_WRITE 					((u16_t)0x07B0)
#define COILS_MAX_AMOUNT_READ 					((u16_t)0x07D0)

typedef struct
{
	u8_t  slaveAddress;
	u8_t  cmd;
	u16_t address;
	u8_t  numberOfBytes;
	u16_t numberOfOutputs;
	u8_t  data[MODBUS_MAX_DATA_SIZE];
	u8_t  length;
}ModbusPdu;

typedef struct
{
	u16_t length;
	u8_t bytes[MODBUS_MAX_ADU_SIZE];
}ModbusAdu;

void ModbusCommon_InitAdu(ModbusAdu* adu);
void ModbusCommon_InitPdu(ModbusPdu* pdu);
u16_t ModbusCommon_CalculateCRC16(ModbusAdu* data);
u8_t ModbusCommon_CheckCRC16(ModbusAdu* data);

#endif /* MODBUSCOMMON_INC_MODBUSCOMMON_H_ */
