/*
 * i2c.h
 *
 *  Created on: 6 авг. 2017 г.
 *      Author: misaka10032
 */

#ifndef I2C_INC_I2C_H_
#define I2C_INC_I2C_H_

#include "stm32f10x_conf.h"

void I2C_HW_Init();
void I2C_StartTransmission(I2C_TypeDef* I2Cx, uint8_t transmissionDirection,  uint8_t slaveAddress);
void I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t I2C_ReadData(I2C_TypeDef* I2Cx);

#endif /* I2C_INC_I2C_H_ */
