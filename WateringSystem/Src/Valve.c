/*
 * Valve.c
 *
 *  Created on: 2 lut 2020
 *      Author: Student235325
 */


#include "../Inc/Valve.h"
#include "main.h"
#include "../sequencer/stm32_seq.h"
#include "../Inc/app_conf.h"

#ifndef __weak
    #define __weak   __attribute__((weak))
#endif /* __weak */

extern SensorData_t sensors;

Watering_Set_t watering_set;

__weak void WateringSetErrorHandler(void)
{
	while(1){}
}

void InitValves(void)
{
	watering_set.p_sensors = &sensors;

	/*valves Init*/
	watering_set.valves[0].id = 0;
	watering_set.valves[0].p_GPIO_port = Valve1_GPIO_Port;
	watering_set.valves[0].GPIO_pin = Valve1_Pin;
	watering_set.valves[0].humidity_limit = 0;

	watering_set.valves[1].id = 1;
	watering_set.valves[1].p_GPIO_port = Valve2_GPIO_Port;
	watering_set.valves[1].GPIO_pin = Valve2_Pin;
	watering_set.valves[1].humidity_limit = 0;

	watering_set.valves[2].id = 2;
	watering_set.valves[2].p_GPIO_port = Valve3_GPIO_Port;
	watering_set.valves[2].GPIO_pin = Valve3_Pin;
	watering_set.valves[2].humidity_limit = 0;

	watering_set.valves[3].id = 3;
	watering_set.valves[3].p_GPIO_port = Valve4_GPIO_Port;
	watering_set.valves[3].GPIO_pin = Valve4_Pin;
	watering_set.valves[3].humidity_limit = 0;

	watering_set.valves[4].id = 4;
	watering_set.valves[4].p_GPIO_port = Valve5_GPIO_Port;
	watering_set.valves[4].GPIO_pin = Valve5_Pin;
	watering_set.valves[4].humidity_limit = 0;

	watering_set.valves[5].id = 5;
	watering_set.valves[5].p_GPIO_port = Valve6_GPIO_Port;
	watering_set.valves[5].GPIO_pin = Valve6_Pin;
	watering_set.valves[5].humidity_limit = 0;

	watering_set.valves[6].id = 6;
	watering_set.valves[6].p_GPIO_port = Valve7_GPIO_Port;
	watering_set.valves[6].GPIO_pin = Valve7_Pin;
	watering_set.valves[6].humidity_limit = 0;

	watering_set.valves[7].id = 7;
	watering_set.valves[7].p_GPIO_port = Valve8_GPIO_Port;
	watering_set.valves[7].GPIO_pin = Valve8_Pin;
	watering_set.valves[7].humidity_limit = 0;

	watering_set.valves[8].id = 8;
	watering_set.valves[8].p_GPIO_port = Valve9_GPIO_Port;
	watering_set.valves[8].GPIO_pin = Valve9_Pin;
	watering_set.valves[8].humidity_limit = 0;

	watering_set.valves[9].id = 9;
	watering_set.valves[9].p_GPIO_port = Valve10_GPIO_Port;
	watering_set.valves[9].GPIO_pin = Valve10_Pin;
	watering_set.valves[9].humidity_limit = 0;

	watering_set.flags = 1;
	watering_set.working_stations = 0;

	UTIL_SEQ_RegTask(1 << CFG_TASK_CONTROL_WATERING_SYSTEM, 0, ControlWateringSystem);
}

void ControlValve(uint8_t valve_id, uint16_t humidity)
{
	if(!watering_set.p_sensors)
	{
		WateringSetErrorHandler();
	}

	if (valve_id < watering_set.p_sensors->avbl_count)
	{
		if(humidity < watering_set.valves[valve_id].humidity_limit)
		{
			HAL_GPIO_WritePin(watering_set.valves[valve_id].p_GPIO_port,
					watering_set.valves[valve_id].GPIO_pin,
					GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(watering_set.valves[valve_id].p_GPIO_port,
					watering_set.valves[valve_id].GPIO_pin,
					GPIO_PIN_RESET);
		}
		return;
	}

	if(valve_id < MAX_STATION_COUNT)
	{
		HAL_GPIO_WritePin(watering_set.valves[valve_id].p_GPIO_port,
				watering_set.valves[valve_id].GPIO_pin,
				GPIO_PIN_RESET);
		return;
	}
}


void ControlWateringSystem(void)
{
	if(!watering_set.p_sensors)
	{
		WateringSetErrorHandler();
	}

	for(uint8_t i = 0; i < MAX_STATION_COUNT; ++i)
	{
		//ControlValve(i, watering_set.p_sensors->data[i]);
		ControlValve(i, SensorCnvUint(i));
	}
	watering_set.flags = 1;
}

void ManageSystem(uint8_t valve_id, uint16_t humidity)
{
	if(valve_id < MAX_STATION_COUNT)
	{
		uint16_t mask = 1 << valve_id;
		watering_set.working_stations &= ~mask;

		if(humidity > 0)
		{
			watering_set.working_stations |= mask;
		}

		if(watering_set.working_stations)
		{
			HAL_GPIO_WritePin(GREEN_LED_Port, GREEN_LED_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GREEN_LED_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
		}
	}
}


