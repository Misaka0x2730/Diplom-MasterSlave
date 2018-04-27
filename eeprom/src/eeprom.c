#include "eeprom.h"

static void EEP_GPIO_Init();
static void EEP_I2C_Init();

static void EEP_GPIO_Init()
{
	GPIO_xInit(PIN_I2C_SDA, GPIO_Speed_50MHz, GPIO_Mode_Out_OD);
	GPIO_xInit(PIN_I2C_SCL, GPIO_Speed_50MHz, GPIO_Mode_Out_OD);
	GPIO_xSetBit(PIN_I2C_SDA);
	GPIO_xSetBit(PIN_I2C_SCL);
	while((GPIO_xReadBit(PIN_I2C_SCL) == Bit_RESET) || (GPIO_xReadBit(PIN_I2C_SDA) == Bit_RESET));
	GPIO_xResetBit(PIN_I2C_SDA);
	while(GPIO_xReadBit(PIN_I2C_SDA) == Bit_SET);
	GPIO_xResetBit(PIN_I2C_SCL);
	while(GPIO_xReadBit(PIN_I2C_SCL) == Bit_SET);
	GPIO_xSetBit(PIN_I2C_SCL);
	while(GPIO_xReadBit(PIN_I2C_SCL) == Bit_RESET);
	GPIO_xSetBit(PIN_I2C_SDA);
	while(GPIO_xReadBit(PIN_I2C_SDA) == Bit_RESET);
	GPIO_xInit(PIN_I2C_SDA, GPIO_Speed_50MHz, GPIO_Mode_AF_OD);
	GPIO_xInit(PIN_I2C_SCL, GPIO_Speed_50MHz, GPIO_Mode_AF_OD);
	#ifdef EEP_GPIO_REMAP
		EEP_GPIO_REMAP;
	#endif
}

static void EEP_I2C_Init()
{
	I2C_InitTypeDef I2C_InitStructure;
	I2C_StructInit(&I2C_InitStructure);

	EEP_I2C_MODULE_RCC_ENABLE;
	I2C_SoftwareResetCmd(I2C1, ENABLE);
	I2C_SoftwareResetCmd(I2C1, DISABLE);
	I2C_DeInit(EEP_I2C_MODULE);

	I2C_InitStructure.I2C_ClockSpeed = EEP_I2C_BAUD;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(EEP_I2C_MODULE, &I2C_InitStructure);
	I2C_Cmd(EEP_I2C_MODULE, ENABLE);
}

void EEP_Init()
{
	EEP_GPIO_Init();
	EEP_I2C_Init();
}

void EEP_WriteByte(u16_t address, u8_t data)
{
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C_Start(EEP_I2C_MODULE,I2C_Direction_Transmitter,EEP_ADDRESS);
	I2C_WriteData(EEP_I2C_MODULE, (address >> 8) & 0xFF);
	I2C_WriteData(EEP_I2C_MODULE, address & 0xFF);
	I2C_WriteData(EEP_I2C_MODULE,data);
	I2C_Stop(EEP_I2C_MODULE);
}

u8_t EEP_ReadByte(u16_t address)
{
	u16_t data = 0;
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C_Start(EEP_I2C_MODULE,I2C_Direction_Transmitter,EEP_ADDRESS);
	I2C_WriteData(EEP_I2C_MODULE, address&0xFF);
	I2C_Start(EEP_I2C_MODULE,I2C_Direction_Receiver,EEP_ADDRESS);
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	data = I2C_ReadData(EEP_I2C_MODULE);
	I2C_Stop(EEP_I2C_MODULE);
	return data;
}

void EEP_WritePage(u16_t address, u8_t* data, u8_t amount)
{
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C_Start(EEP_I2C_MODULE,I2C_Direction_Transmitter,EEP_ADDRESS);
	I2C_WriteData(EEP_I2C_MODULE, address&0xFF);
	u16_t i = 0;
	for(i = 0; (i < amount) && (i < EEP_PAGE_SIZE); i++)
		I2C_WriteData(EEP_I2C_MODULE,data[i]);
	I2C_Stop(EEP_I2C_MODULE);
}

void EEP_ReadPage(u16_t address, u8_t* data, u8_t amount)
{
	u16_t i = 0;
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C_Start(EEP_I2C_MODULE,I2C_Direction_Transmitter,EEP_ADDRESS);
	I2C_WriteData(EEP_I2C_MODULE, address&0xFF);
	I2C_Start(EEP_I2C_MODULE,I2C_Direction_Receiver,EEP_ADDRESS);
	for(i = 0; (i < amount - 1) && (i < EEP_PAGE_SIZE - 1); i++)
		data[i] = I2C_ReadData(EEP_I2C_MODULE);
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	data[i] = I2C_ReadData(EEP_I2C_MODULE);
	I2C_Stop(EEP_I2C_MODULE);
}
