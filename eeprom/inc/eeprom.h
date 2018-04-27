#ifndef __EEPROM_H
#define __EEPROM_H

#include "stm32f107.h"
#include "types.h"
#include "pinMap.h"
#include "i2c.h"
#include "gpio.h"

#define EEP_GPIO_REMAP					do{GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);}while(0)

#if (((EEP_SDA & 0xF0) > 0x40) || ((EEP_SCL & 0xF0) > 0x40))
#error "Incorrect definition for the EEPROM GPIO."
#endif

#define EEP_I2C_MODULE 					I2C1
#define EEP_I2C_MODULE_RCC				RCC_APB1Periph_I2C1
#define EEP_I2C_MODULE_RCC_ENABLE		do{RCC_APB1PeriphClockCmd(EEP_I2C_MODULE_RCC, ENABLE);}while(0)
#define EEP_I2C_BAUD					((u16_t)50000)

#define EEP_PAGE_SIZE					((u16_t)64)

#define EEP_ADDRESS 					((u8_t)0xA0)

#define EEP_NO_CONFIGURATION 			((u8_t)0x00)
#define EEP_YES_CONFIGURATION 			((u8_t)0x01)

void EEP_Init();
void EEP_WriteByte(uint16_t address, u8_t data);
u8_t EEP_ReadByte(uint16_t address);
void EEP_WritePage(uint16_t address, u8_t* data, u8_t amount);
void EEP_ReadPage(u16_t address, u8_t* data, u8_t amount);

#endif //__EEPROM_H
