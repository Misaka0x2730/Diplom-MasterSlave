#ifndef MASTER_INC_MODBUSMASTER_INIT_H_
#define MASTER_INC_MODBUSMASTER_INIT_H_

#include "stm32f107.h"
#include "ModbusCommon.h"
#include "device.h"
#include "gpio.h"
#include "delay.h"

typedef struct
{
	u8_t state;
	u32_t rxDelay;
	u32_t rxTimer;
	u32_t txDelay;
	u32_t txTimer;
	u16_t slaveAddress;
	ModbusAdu rxData;
	ModbusAdu txData;
}Modbus_Master_State;

void ModbusMaster_State_Init();
void ModbusMaster_Init_GPIO();
void ModbusMaster_Init_USART();
void ModbusMaster_Init_Timer();
void ModbusMaster_Init_DMA();
void ModbusMaster_Init_NVIC();
void ModbusMaster_Start();
void ModbusMaster_Stop();
void ModbusMaster_SendReadInputMessage();
u8_t ModbusMaster_Send(ModbusAdu* data);

#endif /* MASTER_INC_MODBUSMASTER_INIT_H_ */
