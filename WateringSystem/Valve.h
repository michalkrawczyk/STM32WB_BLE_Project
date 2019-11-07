/*
 * Valve.h
 *
 *  Created on: 7 lis 2019
 *      Author: Student235325
 */

#ifndef VALVE_H_
#define VALVE_H_

#include "main.h"
#include "watering_common.hpp"

class Valve {
	GPIO_TypeDef *m_port_ptr;
	GPIO_Pin m_pin;

	float m_desired_humidity;
	bool m_available;
	bool m_opened;

	void openValve();
	void closeValve();

public:
	Valve(GPIO_TypeDef *port, GPIO_Pin pin);

	void setDesiredHumidity(const float &humidity);
	float getDesiredHumidity() const;

	void setAvailable(const bool &available);
	bool isAvailable() const;

	bool isOpened() const;


	void valveControl(const float &current_humidity);


};

Valve valves[MAX_STATION_COUNT] = {
		Valve(Valve1_GPIO_Port, Valve1_Pin),
		Valve(Valve2_GPIO_Port, Valve2_Pin),
		Valve(Valve3_GPIO_Port, Valve3_Pin),
		Valve(Valve4_GPIO_Port, Valve4_Pin),
		Valve(Valve5_GPIO_Port, Valve5_Pin),
		Valve(Valve6_GPIO_Port, Valve6_Pin),
		Valve(Valve7_GPIO_Port, Valve7_Pin),
		Valve(Valve8_GPIO_Port, Valve8_Pin),
		Valve(Valve9_GPIO_Port, Valve9_Pin),
		Valve(Valve10_GPIO_Port, Valve10_Pin),
};



#endif /* VALVE_H_ */

