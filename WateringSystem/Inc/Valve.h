/*
 * Valve.h
 *
 *  Created on: 2 lut 2020
 *      Author: Student235325
 */

#ifndef INC_VALVE_H_
#define INC_VALVE_H_

#include "../Inc/HumiditySensor.h"
#include "main.h"



typedef struct
{
	GPIO_TypeDef *p_GPIO_port;
	uint16_t GPIO_pin;
	uint8_t id;

	uint16_t humidity_limit;
}Valve_t;

typedef struct
{
	Valve_t valves[MAX_STATION_COUNT];
	SensorData_t *p_sensors;

	uint8_t flag;
}Watering_Set_t;

extern Watering_Set_t watering_set;

void InitValves(void);
void ControlValve(uint8_t valve_id, uint16_t humidity);
void ControlWateringSystem(void);



#endif /* INC_VALVE_H_ */
