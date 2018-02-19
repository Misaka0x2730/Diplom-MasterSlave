#ifndef SENSORS_INC_SENSORS_H_
#define SENSORS_INC_SENSORS_H_

#include "types.h"
#include "bmp085.h"
#include "sht21.h"

struct
{
	u16_t temperature;
	u16_t pressure;
	u16_t hymidity;
}SensorsData;

#endif /* SENSORS_INC_SENSORS_H_ */
