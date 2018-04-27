#ifndef MASTER_INC_MODBUSMASTER_H_
#define MASTER_INC_MODBUSMASTER_H_

#include "stm32f107.h"
#include "inputsOutputs.h"
#include "ModbusMaster_Init.h"

void ModbusMaster_ParsingPdu(ModbusPdu* pdu, ModbusAdu* adu);
u8_t ModbusMaster_Parsing(u16_t slaveAddress, ModbusAdu* adu);
void ModbusMaster_SendReadInputMessage();

#endif /* MASTER_INC_MODBUSMASTER_H_ */
