/*
 * eeprom.c
 *
 *  Created on: 6 авг. 2017 г.
 *      Author: misaka10032
 */

#include "eeprom.h"

uint8_t EEP_ReadByte(uint8_t address)
{
	uint8_t data = 0;
	I2C_StartTransmission(I2C1, I2C_Direction_Transmitter, EEP_ADDRESS);
	I2C_WriteData(I2C1, address);
	I2C_StartTransmission(I2C1, I2C_Direction_Receiver, EEP_ADDRESS);
	data = I2C_ReadData(I2C1);
	I2C_GenerateSTOP(I2C1, ENABLE);
	return data;
}

void EEP_WriteByte(uint8_t address, uint8_t data)
{
	I2C_StartTransmission(I2C1, I2C_Direction_Transmitter, EEP_ADDRESS);
	I2C_WriteData(I2C1, address);
	I2C_WriteData(I2C1, data);
	I2C_GenerateSTOP(I2C1, ENABLE);
}


