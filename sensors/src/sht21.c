#include "sht21.h"

static s16_t temperature = 0;
static u16_t rh = 0;

void SHT21_Init(I2C_TypeDef *i2cModule)
{
	I2C_Start(i2cModule, I2C_Direction_Transmitter, SHT21_ADDRESS);
	I2C_WriteData(i2cModule, SHT21_SOFT_RESET);
	I2C_Stop(i2cModule);
	delay_ms(100);
}

void SHT21_StartMeasureTemperature(I2C_TypeDef *i2cModule)
{
	I2C_Start(i2cModule, I2C_Direction_Transmitter, SHT21_ADDRESS);
	I2C_WriteData(i2cModule, SHT21_START_TEMP_MEASURE);
	I2C_Stop(i2cModule);
}

void SHT21_StartMeasureRH(I2C_TypeDef *i2cModule)
{
	I2C_Start(i2cModule, I2C_Direction_Transmitter, SHT21_ADDRESS);
	I2C_WriteData(i2cModule, SHT21_START_RH_MEASURE);
	I2C_Stop(i2cModule);
}

s16_t SHT21_GetTemp()
{
	return temperature;
}

u16_t SHT21_GetRH()
{
	return rh;
}

void SHT21_CalculateTemp(I2C_TypeDef *i2cModule)
{
	s16_t data = 0;
	float result = 0.0f;
	I2C_Start(i2cModule, I2C_Direction_Receiver, SHT21_ADDRESS);
	data = (I2C_ReadData(i2cModule)<<8);
	data += I2C_ReadData(i2cModule);
	I2C_Stop(i2cModule);
	result = (float)data/65536.0;
	result = result*175.72f;
	result -= 46.85f;
	result *= 10;
	data = floorf(result);
	temperature = data;
}

void SHT21_CalculateRH(I2C_TypeDef *i2cModule)
{
	s16_t data = 0;
	float result = 0.0f;
	I2C_Start(i2cModule, I2C_Direction_Receiver, SHT21_ADDRESS);
	data = (I2C_ReadData(i2cModule)<<8);
	data += I2C_ReadData(i2cModule);
	I2C_Stop(i2cModule);
	result = (float)data/65536.0;
	result = result*125.0f;
	result -= 6.0f;
	result *= 10;
	data = floorf(result);
	rh = data;
}
