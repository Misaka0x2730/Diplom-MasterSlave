#include "i2c.h"

void I2C_HW_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;

	I2C_SoftwareResetCmd(I2C1, ENABLE);
	I2C_SoftwareResetCmd(I2C1, DISABLE);
	I2C_DeInit(I2C1);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_Remap_I2C1);

	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
}

void I2C_StartTransmission(I2C_TypeDef* I2Cx, uint8_t transmissionDirection, uint8_t slaveAddress)
{
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

    I2C_GenerateSTART(I2Cx, ENABLE);

    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2Cx, slaveAddress, transmissionDirection);

    if(transmissionDirection == I2C_Direction_Transmitter)
    	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    if(transmissionDirection == I2C_Direction_Receiver)
    	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

}

void I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data)
{
    I2C_SendData(I2Cx, data);
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

uint8_t I2C_ReadData(I2C_TypeDef* I2Cx)
{
    uint8_t data;
    while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
    data = I2C_ReceiveData(I2Cx);
    return data;
}
