/*
 * DHT11.cpp
 *
 *  Created on: 21 paź 2019
 *      Author: Student235325
 */

#include "DHT11.hpp"
#include "../DHT11/DWT/dwt.h"

DHT11::DHT11(GPIO_TypeDef *port, uint16_t pin , GPIO_InitTypeDef &GPIO_InitStruct):
m_port_ptr(port),
m_pin(pin),
m_GPIO_InitStruct_ptr(&GPIO_InitStruct),
m_humidity_integral(0u),
m_humidity_decimal(0u),
m_temperature_integral(0u),
m_temperature_decimal(0u)
{
}


void DHT11::mSetInputGPIO()
{
	m_GPIO_InitStruct_ptr->Mode = GPIO_MODE_INPUT;
	m_GPIO_InitStruct_ptr->Pin = m_pin;
	m_GPIO_InitStruct_ptr->Pull = GPIO_NOPULL;

	HAL_GPIO_Init(m_port_ptr, m_GPIO_InitStruct_ptr);
}

void DHT11::mSetOutputGPIO()
{
	m_GPIO_InitStruct_ptr->Pin = m_pin; // or |=
	m_GPIO_InitStruct_ptr->Mode = GPIO_MODE_OUTPUT_PP;
	m_GPIO_InitStruct_ptr->Pull = GPIO_NOPULL;
	m_GPIO_InitStruct_ptr->Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(m_port_ptr, m_GPIO_InitStruct_ptr);
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
		delayUS_ASM(60u);
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



uint16_t DHT11::getHumidity() const
{
	return ((m_humidity_integral << 8) | m_humidity_decimal);
}

uint16_t DHT11::getTemperature() const
{
	return ((m_temperature_integral << 8) | m_temperature_decimal);
}

void DHT11::init()
{
	DHT11::mSetOutputGPIO();

	HAL_GPIO_WritePin(m_port_ptr, m_pin, GPIO_PIN_RESET);
	delayUS_ASM(18000u);

	DHT11::mSetInputGPIO();
}

bool DHT11::checkResponse()
{
	//Add timeout?
	DHT11::init();

	delayUS_ASM(40u); //Delay for 40us
	if(HAL_GPIO_ReadPin(m_port_ptr, m_pin) == GPIO_PIN_RESET)
	{
		delayUS_ASM(80u);	//Delay for 80us
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

bool DHT11::isDataCorrect(const uint8_t &check_bit)
{
	uint16_t checksum = m_humidity_integral + m_humidity_decimal + m_temperature_integral + m_temperature_integral;
	uint8_t lsb = (uint8_t)(checksum & 0b1111);

	return (0xF - lsb) == check_bit; //TODO:Check

}
