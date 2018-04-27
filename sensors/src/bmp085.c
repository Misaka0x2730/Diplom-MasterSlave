#include "bmp085.h"

static s16_t AC1 = 0, AC2 = 0, AC3 = 0, B1 = 0, B2 = 0, MB = 0, MC = 0, MD = 0;
static u16_t AC4 = 0, AC5 = 0, AC6 = 0;
static u32_t pressure = 0;
static u16_t UT = 0;
static u32_t UP = 0;

void BMP085_Init(I2C_TypeDef *i2cModule)
{
	AC1 = BMP085_ReadSignedCoefficient(i2cModule, BMP085_COEFFICIENT_AC1_ADDRESS);
	AC2 = BMP085_ReadSignedCoefficient(i2cModule, BMP085_COEFFICIENT_AC2_ADDRESS);
	AC3 = BMP085_ReadSignedCoefficient(i2cModule, BMP085_COEFFICIENT_AC3_ADDRESS);
	AC4 = BMP085_ReadUnsignedCoefficient(i2cModule, BMP085_COEFFICIENT_AC4_ADDRESS);
	AC5 = BMP085_ReadUnsignedCoefficient(i2cModule, BMP085_COEFFICIENT_AC5_ADDRESS);
	AC6 = BMP085_ReadUnsignedCoefficient(i2cModule, BMP085_COEFFICIENT_AC6_ADDRESS);
	B1  = BMP085_ReadSignedCoefficient(i2cModule, BMP085_COEFFICIENT_B1_ADDRESS);
	B2  = BMP085_ReadSignedCoefficient(i2cModule, BMP085_COEFFICIENT_B2_ADDRESS);
	MB  = BMP085_ReadSignedCoefficient(i2cModule, BMP085_COEFFICIENT_MB_ADDRESS);
	MC  = BMP085_ReadSignedCoefficient(i2cModule, BMP085_COEFFICIENT_MC_ADDRESS);
	MD  = BMP085_ReadSignedCoefficient(i2cModule, BMP085_COEFFICIENT_MD_ADDRESS);
}

u16_t BMP085_ReadTemperature(I2C_TypeDef *i2cModule)
{
	u16_t data = 0;
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C_Start(i2cModule, I2C_Direction_Transmitter, BMP085_ADDRESS);
	I2C_WriteData(i2cModule, BMP085_MSB_ADDRESS);
	I2C_Start(i2cModule, I2C_Direction_Receiver, BMP085_ADDRESS);
	data = (I2C_ReadData(i2cModule)<<8);
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	data += I2C_ReadData(i2cModule);
	I2C_Stop(i2cModule);
	return data;
}

u32_t BMP085_ReadPressure(I2C_TypeDef *i2cModule)
{
	u32_t data = 0;
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C_Start(i2cModule, I2C_Direction_Transmitter, BMP085_ADDRESS);
	I2C_WriteData(i2cModule, BMP085_MSB_ADDRESS);
	I2C_Start(i2cModule, I2C_Direction_Receiver, BMP085_ADDRESS);
	data = (I2C_ReadData(i2cModule)<<16);
	data += (I2C_ReadData(i2cModule)<<8);
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	data += I2C_ReadData(i2cModule);
	I2C_Stop(i2cModule);
	return data;
}

static s16_t BMP085_ReadSignedCoefficient(I2C_TypeDef *i2cModule, u16_t address)
{
	s16_t data = 0;
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C_Start(i2cModule, I2C_Direction_Transmitter, BMP085_ADDRESS);
	I2C_WriteData(i2cModule, ((address)>>8) & 0xFF);
	I2C_WriteData(i2cModule, (address & 0xFF));
	I2C_Start(i2cModule, I2C_Direction_Receiver, BMP085_ADDRESS);
	data = (I2C_ReadData(i2cModule)<<8);
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	data += I2C_ReadData(i2cModule);
	I2C_Stop(i2cModule);
	return data;
}

static u16_t BMP085_ReadUnsignedCoefficient(I2C_TypeDef *i2cModule, u16_t address)
{
	u16_t data = 0;
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C_Start(i2cModule, I2C_Direction_Transmitter, BMP085_ADDRESS);
	I2C_WriteData(i2cModule, ((address)>>8) & 0xFF);
	I2C_WriteData(i2cModule, (address & 0xFF));
	I2C_Start(i2cModule, I2C_Direction_Receiver, BMP085_ADDRESS);
	data = (I2C_ReadData(i2cModule)<<8);
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	data += I2C_ReadData(i2cModule);
	I2C_Stop(i2cModule);
	return data;
}

void BMP085_StartMeasureTemperature(I2C_TypeDef *i2cModule)
{
	I2C_Start(i2cModule, I2C_Direction_Transmitter, BMP085_ADDRESS);
	I2C_WriteData(i2cModule, BMP085_CONTROL_ADDRESS);
	I2C_WriteData(i2cModule, BMP085_START_TEMP_MEASURE);
	I2C_Stop(i2cModule);
}

void BMP085_StartMeasurePressure(I2C_TypeDef *i2cModule)
{
	I2C_Start(i2cModule, I2C_Direction_Transmitter, BMP085_ADDRESS);
	I2C_WriteData(i2cModule, BMP085_CONTROL_ADDRESS);
	I2C_WriteData(i2cModule, BMP085_START_PRESSURE_MEASURE_OSS_1);
	I2C_Stop(i2cModule);
}

void BMP085_ReadUT(I2C_TypeDef *i2cModule)
{
	u16_t data = 0;
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C_Start(i2cModule, I2C_Direction_Transmitter, BMP085_ADDRESS);
	I2C_WriteData(i2cModule, BMP085_MSB_ADDRESS);
	I2C_Start(i2cModule, I2C_Direction_Receiver, BMP085_ADDRESS);
	data = (I2C_ReadData(i2cModule)<<8);
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	data += I2C_ReadData(i2cModule);
	I2C_Stop(i2cModule);
	UT = data;
}

void BMP085_ReadUP(I2C_TypeDef *i2cModule)
{
	u32_t data = 0;
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C_Start(i2cModule, I2C_Direction_Transmitter, BMP085_ADDRESS);
	I2C_WriteData(i2cModule, BMP085_MSB_ADDRESS);
	I2C_Start(i2cModule, I2C_Direction_Receiver, BMP085_ADDRESS);
	data = (I2C_ReadData(i2cModule)<<16);
	data += (I2C_ReadData(i2cModule)<<8);
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	data += I2C_ReadData(i2cModule);
	I2C_Stop(i2cModule);
	UP = data>>(8-1);//oss = 1
}

u32_t BMP085_GetPressure()
{
	return pressure;
}

void BMP085_CalculatePressure()
{
	u8_t oss = 1;
	u16_t trueTemperature = 0;
	u32_t truePressure = 0;
	u32_t B4 = 0, B7 = 0;
	s32_t X1 = 0, X2 = 0, X3 = 0, B3 = 0, B5 = 0, B6 = 0;

	/* Calculate true temperature */
	X1 = ((UT-AC6)*AC5)>>15;
	X2 = (MC<<11)/(X1+MD);
	B5 = X1+X2;
	trueTemperature = (B5+8)>>4;

	/* Calculate true pressure */
	B6 = B5 - 4000;
	X1 = (B2*((B6*B6)>>12))>>11;
	X2 = (AC2*B6)>>11;
	X3 = X1+X2;
	B3 = (((AC1*4+X3)<<oss)+2)/4;
	X1 = AC3*B6>>13;
	X2 = (B1*((B6*B6)>>12))>>16;
	X3 = ((X1+X2)+2)>>2;
	B4 = (AC4*(u32_t)(X3+32768))>>15;
	B7 = ((u32_t)UP-B3)*(50000>>oss);
	if(B7 < 0x80000000)
	{
		truePressure = (B7*2)/B4;
	}
	else
	{
		truePressure = (B7/B4)*2;
	}
	X1 = (truePressure>>8)*(truePressure>>8);
	X1 = (X1*3038)>>16;
	X2 = (-7357*truePressure)>>16;
	truePressure = truePressure+((X1+X2+3791)>>4);
	pressure = truePressure;
}
