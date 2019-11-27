/*
 * DHT11.hpp
 *
 *  Created on: 21 paź 2019
 *      Author: Student235325
 */

#ifndef DHT11_HPP_
#define DHT11_HPP_

#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_gpio.h"
//#include "stm32wb55xx.h"
#include "main.h"

class DHT11 {
	GPIO_TypeDef *m_port_ptr;
	uint16_t m_pin;
	GPIO_InitTypeDef *m_GPIO_InitStruct_ptr;


	uint8_t m_humidity_integral,
			m_humidity_decimal,
			m_temperature_integral,
			m_temperature_decimal;


	void mSetInputGPIO();
	void mSetOutputGPIO();

	void readData(uint8_t &data);

public:
	DHT11(GPIO_TypeDef *port, uint16_t pin , GPIO_InitTypeDef &GPIO_InitStruct);

	uint16_t getHumidity() const;
	uint16_t getTemperature() const;

	void init();

	bool checkResponse();
	bool makeMeasure();
	bool isDataCorrect(const uint8_t &parity_bit);


};


#endif /* DHT11_HPP_ */
