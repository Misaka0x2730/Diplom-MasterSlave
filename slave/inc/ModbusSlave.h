#ifndef __MODBUSSLAVE_H
#define __MODBUSSLAVE_H

#include "ModbusSlave_Init.h"
#include "stm32f107.h"
#include "inputsOutputs.h"

#define MODBUS_START_SAFE_PARSING			0//SYS_PROTECT
#define MODBUS_STOP_SAFE_PARSING			//SYS_UNPROTECT

void ModbusSlave_ParsingPdu(ModbusPdu* pdu, ModbusAdu* adu);
u8_t ModbusSlave_Parsing(u16_t slaveAddress, ModbusAdu* adu);

#endif //__MODBUSSLAVE_H
