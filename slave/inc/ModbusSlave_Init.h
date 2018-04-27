#ifndef __MODBUS_RTU_H
#define __MODBUS_RTU_H

#include "stm32f107.h"
#include "stm32f10x_conf.h"
#include "gpio.h"
#include "pinMap.h"
#include "device.h"
#include "ModbusCommon.h"

void ModbusSlave_State_Init();
void ModbusSlave_Init_GPIO();
void ModbusSlave_Init_USART();
void ModbusSlave_Init_Timer();
void ModbusSlave_Init_DMA();
void ModbusSlave_Init_NVIC();

void ModbusSlave_Start();
void ModbusSlave_Stop();
u8_t ModbusSlave_CheckCorrectData();
u8_t ModbusSlave_CheckCRC16(ModbusAdu* data);
u8_t ModbusSlave_Send(ModbusAdu* data);
u8_t ModbusSlave_SendResponse(ModbusPdu* pdu, u8_t errorCode);
void ModbusSlave_GetDeviceInfo(ModbusPdu* pdu);

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
}Modbus_Slave_State;

#endif //__MODBUS_RTU_H
