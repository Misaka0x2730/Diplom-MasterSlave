#ifndef SENSORS_INC_SHT21_H_
#define SENSORS_INC_SHT21_H_

#include "stm32f10x_conf.h"
#include "types.h"
#include "i2c.h"
#include "delay.h"
#include <math.h>

#define SHT21_ADDRESS				0x80
#define SHT21_SOFT_RESET			0xFE
#define SHT21_START_TEMP_MEASURE	0xF3
#define SHT21_START_RH_MEASURE		0xF5

void SHT21_Init(I2C_TypeDef *i2cModule);
void SHT21_StartMeasure(I2C_TypeDef *i2cModule);
s16_t SHT21_GetTemp();
u16_t SHT21_GetRH();
void SHT21_StartMeasureTemperature(I2C_TypeDef *i2cModule);
void SHT21_StartMeasureRH(I2C_TypeDef *i2cModule);
void SHT21_CalculateTemp(I2C_TypeDef *i2cModule);
void SHT21_CalculateRH(I2C_TypeDef *i2cModule);

#endif /* SENSORS_INC_SHT21_H_ */
