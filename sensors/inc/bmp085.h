#ifndef SENSORS_INC_BMP085_H_
#define SENSORS_INC_BMP085_H_

#include "stm32f10x_conf.h"
#include "types.h"
#include "i2c.h"
#include "delay.h"
#include <math.h>

#define BMP085_ADDRESS							0xEE
#define BMP085_COEFFICIENT_AC1_ADDRESS			0xAAAB
#define BMP085_COEFFICIENT_AC2_ADDRESS			0xACAD
#define BMP085_COEFFICIENT_AC3_ADDRESS			0xAEAF
#define BMP085_COEFFICIENT_AC4_ADDRESS			0xB0B1
#define BMP085_COEFFICIENT_AC5_ADDRESS			0xB2B3
#define BMP085_COEFFICIENT_AC6_ADDRESS			0xB4B5
#define BMP085_COEFFICIENT_B1_ADDRESS			0xB6B7
#define BMP085_COEFFICIENT_B2_ADDRESS			0xB8B9
#define BMP085_COEFFICIENT_MB_ADDRESS			0xBABB
#define BMP085_COEFFICIENT_MC_ADDRESS			0xBCBD
#define BMP085_COEFFICIENT_MD_ADDRESS			0xBEBF
#define BMP085_CONTROL_ADDRESS					0xF4
#define BMP085_MSB_ADDRESS						0xF6
#define BMP085_LSB_ADDRESS						0xF7
#define BMP085_XLSB_ADDRESS						0xF8

#define BMP085_START_TEMP_MEASURE				0x2E
#define BMP085_START_PRESSURE_MEASURE_OSS_0		0x34
#define BMP085_START_PRESSURE_MEASURE_OSS_1		0x74
#define BMP085_START_PRESSURE_MEASURE_OSS_2		0xB4
#define BMP085_START_PRESSURE_MEASURE_OSS_3		0xF4

static s16_t BMP085_ReadSignedCoefficient(I2C_TypeDef *i2cModule, u16_t address);
static u16_t BMP085_ReadUnsignedCoefficient(I2C_TypeDef *i2cModule, u16_t address);

void BMP085_Init(I2C_TypeDef *i2cModule);
u32_t BMP085_GetPressure();
void BMP085_CalculatePressure();
void BMP085_StartMeasureTemperature(I2C_TypeDef *i2cModule);
void BMP085_StartMeasurePressure(I2C_TypeDef *i2cModule);
void BMP085_ReadUP(I2C_TypeDef *i2cModule);
void BMP085_ReadUT(I2C_TypeDef *i2cModule);
#endif /* SENSORS_INC_BMP085_H_ */
