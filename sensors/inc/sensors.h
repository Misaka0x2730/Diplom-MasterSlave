#ifndef SENSORS_INC_SENSORS_H_
#define SENSORS_INC_SENSORS_H_

#include "types.h"
#include "bmp085.h"
#include "sht21.h"
#include "inputsOutputs.h"
#include "stm32f107.h"
#include "device.h"

#define SENSORS_I2C_MODULE	I2C1

void Sensors_Init();
void Sensor_Exec();

#endif /* SENSORS_INC_SENSORS_H_ */
