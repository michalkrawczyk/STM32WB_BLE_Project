/*
 * DHT11.cpp
 *
 *  Created on: 21 paź 2019
 *      Author: Student235325
 */

#include "DHT11.hpp"


DHT11::DHT11(GPIO_TypeDef *port, GPIO_Pin pin , GPIO_InitTypeDef &GPIO_InitStruct):
m_port_ptr(*port),
m_pin(pin),
m_GPIO_InitStruct_ptr(&GPIO_InitStruct),
m_humidity_integral(0u),
m_humidity_decimal(0u),
m_temperature_integral(0u),
m_temperature_decimal(0u)
{
}

//Todo:check arguments of HAL functions

void DHT11::mSetInputGPIO()
{
	m_GPIO_InitStruct_ptr->Mode = GPIO_MODE_INPUT;
	m_GPIO_InitStruct_ptr->Pin = m_pin;
	m_GPIO_InitStruct_ptr->Pull = GPIO_NOPULL;

	GPIO_Init(*m_port_ptr, m_GPIO_InitStruct_ptr); //TODO:check
}

void DHT11::mSetOutputGPIO()
{
	m_GPIO_InitStruct_ptr->Pin = m_pin; // or |=
	m_GPIO_InitStruct_ptr->Mode = GPIO_MODE_OUTPUT_PP;
	m_GPIO_InitStruct_ptr->Pull = GPIO_NOPULL;
	m_GPIO_InitStruct_ptr->Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_Init(*m_port_ptr, m_GPIO_InitStruct_ptr);
}

void DHT11::readData(uint8_t &data)
{
	data = 0u;

	for(uint8_t i = 0; i < 8; ++i)
	{
		while(HAL_GPIO_ReadPin(m_port_ptr, m_pin) == GPIO_PIN_RESET);
		/*
		 * 70 us voltage length means bit=1
		 * 26-28 us voltage length means bit=0
		 *so after e.g 60us if voltage is high it means bit=1
		 *  */
		DWT_Delay(60u);
		if(HAL_GPIO_ReadPin(m_port_ptr, m_pin) == GPIO_PIN_SET)
		{
			data |= (1 << (7 - i)); 		// bit=1
		}
		else
		{
			data &= ~(1 << (7 - i)); 	// bit=0
		}

		while(HAL_GPIO_ReadPin(m_port_ptr, m_pin) == GPIO_PIN_SET);

	}

}



float DHT11::getHumidity()
{
	return m_humidity_integral + (m_humidity_decimal / 1000.);
}

float DHT11::getTemperature()
{
	return m_temperature_integral + (m_temperature_decimal / 1000.);
}

void DHT11::init()
{
	DHT11::mSetOutputGPIO();

	HAL_GPIO_WritePin(m_port_ptr, m_pin, GPIO_PIN_RESET);
	DWT_Delay(18000u);

	DHT11::mSetInputGPIO();
}

bool DHT11::checkResponse()
{
	//Add timeout?
	DHT11::init();

	DWT_Delay(40u); //Delay for 40us
	if(HAL_GPIO_ReadPin(m_port_ptr, m_pin) == GPIO_PIN_RESET)
	{
		DWT_Delay(80u);	//Delay for 80us
		if (HAL_GPIO_ReadPin(m_port_ptr, m_pin) == GPIO_PIN_SET)
		{
			while(HAL_GPIO_ReadPin(m_port_ptr, m_pin) == GPIO_PIN_SET);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}




bool DHT11::makeMeasure()
{
	uint8_t parity_bit;

	if(DHT11::checkResponse())
	{
		DHT11::readData(m_humidity_integral);
		DHT11::readData(m_humidity_decimal);

		DHT11::readData(m_temperature_integral);
		DHT11::readData(m_temperature_decimal);

		DHT11::readData(parity_bit);

		if(!isDataCorrect(parity_bit))
		{
			m_humidity_integral = 0u;
			m_humidity_decimal = 0u;

			m_temperature_integral = 0u;
			m_temperature_decimal = 0u;

			return false;
		}
	}

	return false;
}

bool DHT11::isDataCorrect(uint8_t &parity_bit)
{
	return ((m_humidity_integral ^ m_humidity_decimal
			^ m_temperature_integral
			^ m_temperature_decimal) == parity);
	//TODO: Rewrite - User manuals from DHT11 think that check sum and parity bit is the same ...
}
