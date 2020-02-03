/*
 * HumiditySensor.c
 *
 *  Created on: 1 lut 2020
 *      Author: Student235325
 */


#include "../Inc/HumiditySensor.h"
#include "../../Inc/main.h"

extern ADC_HandleTypeDef hadc1;

SensorData_t sensors;

static uint16_t CountFix(uint16_t count)
{
	if(count > MAX_STATION_COUNT)
		return MAX_STATION_COUNT;
	return count;
}

HAL_StatusTypeDef SensorsInit(uint16_t count)
{
	sensors.avbl_count = CountFix(count);
	return HAL_ADC_Start_DMA(&hadc1, (uint32_t*) sensors.data, MAX_STATION_COUNT);
}


float SensorCnvFloat(uint8_t index)
{
	if(index >= MAX_STATION_COUNT || index >= sensors.avbl_count)
		return 0;
	float val = (float)(4096 - sensors.data[index]) * 100 / (4096-100);
	return val;

}
uint16_t SensorCnvUint(uint8_t index)
{
	return (uint16_t)(SensorCnvFloat(index));
}
