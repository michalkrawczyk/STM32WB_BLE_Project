/*
 * Valve.cpp
 *
 *  Created on: 7 lis 2019
 *      Author: Student235325
 */

#include "Valve.h"

Valve::Valve(GPIO_TypeDef *port, GPIO_Pin pin):
m_port_ptr(port),
m_pin(pin),
m_desired_humidity(0.f),
m_available(false),
m_opened(false)
{
}

void Valve::openValve() {
	HAL_GPIO_WritePin(m_port_ptr, m_pin, GPIO_PIN_SET);
	m_opened = true;
}

void Valve::closeValve() {
	HAL_GPIO_WritePin(m_port_ptr, m_pin, GPIO_PIN_RESET);
	m_opened = false;
}


void Valve::setDesiredHumidity(const float &humidity) {
	m_desired_humidity = humidity;
}

float Valve::getDesiredHumidity() const {
	return m_desired_humidity;
}

void Valve::setAvailable(const bool &available) {
	m_available = available;
}

bool Valve::isAvailable() const {
	return m_available;
}

bool Valve::isOpened() const {
	return m_opened;
}


void Valve::valveControl(const float &current_humidity) {
	//TODO: consider adding tolerance
	if(m_available)
	{
		if (current_humidity < m_desired_humidity && !m_opened)
			openValve();
		else if (current_humidity >= m_desired_humidity && m_opened)
			closeValve();
	}
}
