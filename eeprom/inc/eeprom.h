/*
 * eeprom.h
 *
 *  Created on: 6 авг. 2017 г.
 *      Author: misaka10032
 */

#ifndef EEPROM_INC_EEPROM_H_
#define EEPROM_INC_EEPROM_H_

#include <stdint.h>
#include "i2c.h"

#define EEP_ADDRESS	0xA0

uint8_t EEP_ReadByte(uint8_t address);
void EEP_WriteByte(uint8_t address, uint8_t data);

#endif /* EEPROM_INC_EEPROM_H_ */
