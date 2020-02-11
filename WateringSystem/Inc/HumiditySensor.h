/*
 * HumiditySensor.h
 *
 *  Created on: 1 lut 2020
 *      Author: Student235325
 */

#ifndef INC_HUMIDITYSENSOR_H_
#define INC_HUMIDITYSENSOR_H_

#ifndef MAX_STATION_COUNT
    #define MAX_STATION_COUNT 10
#endif

#ifndef HAL_StatusTypeDef
#include "main.h"
#endif

#ifndef uint16_t
#include <stdint.h>
#endif


typedef struct
{
	uint16_t data[MAX_STATION_COUNT];
	uint16_t avbl_count;
}SensorData_t;

extern SensorData_t sensors;

HAL_StatusTypeDef SensorsInit(uint16_t count);
float SensorCnvFloat(uint8_t index);
uint16_t SensorCnvUint(uint8_t index);


#endif /* INC_HUMIDITYSENSOR_H_ */
