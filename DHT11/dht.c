/*
 * dht.c
 *
 *  Created on: 27 lis 2019
 *      Author: Student235325
 */

#include "dht.h"

#include "../DHT11/DWT/dwt_stm32_delay.h"
#include "stdbool.h"
#include "stdint.h"
#include "main.h"
extern GPIO_InitTypeDef GPIO_InitStruct;


//Private Function Prototypes
/////////////////////////////////////////////////

static void setInputGPIO(void);
static void setOutputGPIO(void);

static void initMeasureDHT(void);

static bool checkResponseDHT(void);

static void readData(uint8_t data);

static bool isDataCorrect(uint8_t check_bit);

//Public Functions
/////////////////////////////////////////////////
void initDHT(void)
{
	dht.humidity_decimal = 0;
	dht.humidity_integral = 0;
	dht.parity_bit = 0;
	dht.temperature_decimal = 0;
	dht.temperature_integral = 0;
}

bool makeMeasureDHT(void)
{

		if(checkResponseDHT())
		{
			readData(dht.humidity_integral);
			readData(dht.humidity_decimal);

			readData(dht.temperature_integral);
			readData(dht.temperature_decimal);

			readData(dht.parity_bit);

//			if(!isDataCorrect(dht.parity_bit))
//			{
//				dht.humidity_integral = 0u;
//				dht.humidity_decimal = 0u;
//
//				dht.temperature_integral = 0u;
//				dht.temperature_decimal = 0u;
//
//				return false;
//			}
		}

		return true;
}

uint16_t getTemperature()
{
	return ((uint16_t)(dht.temperature_integral << 8) + dht.temperature_decimal);
}

uint16_t getHumidity()
{
	return ((uint16_t)(dht.humidity_integral << 8 ) + dht.humidity_decimal);
}


//Private Functions
/////////////////////////////////////////////////
static void setInputGPIO(void)
{
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pin = DHT11_PIN_Pin;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(DHT11_PIN_GPIO_Port, &GPIO_InitStruct);
}

static void setOutputGPIO(void)
{
	GPIO_InitStruct.Pin = DHT11_PIN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(DHT11_PIN_GPIO_Port, &GPIO_InitStruct);
}

static void initMeasureDHT(void)
{
	setOutputGPIO();

	HAL_GPIO_WritePin(DHT11_PIN_GPIO_Port, DHT11_PIN_Pin, GPIO_PIN_RESET);
	DWT_Delay_us(18000);

	setInputGPIO();
}

static bool checkResponseDHT(void)
{
	//Add timeout?
		initMeasureDHT();

		DWT_Delay_us(40u); //Delay for 40us
		if(HAL_GPIO_ReadPin(DHT11_PIN_GPIO_Port, DHT11_PIN_Pin) == GPIO_PIN_RESET)
		{
			DWT_Delay_us(80u);	//Delay for 80us
			if (HAL_GPIO_ReadPin(DHT11_PIN_GPIO_Port, DHT11_PIN_Pin) == GPIO_PIN_SET)
			{
				while(HAL_GPIO_ReadPin(DHT11_PIN_GPIO_Port, DHT11_PIN_Pin) == GPIO_PIN_SET);
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


static bool isDataCorrect(uint8_t check_bit)
{
	uint16_t checksum = dht.humidity_integral + dht.humidity_decimal + dht.temperature_integral + dht.temperature_integral;
	uint8_t lsb = (uint8_t)(checksum & 0b1111);

	return (0xF - lsb) == check_bit; //TODO:Check
}

static void readData(uint8_t data)
{
	data = 0u;

		for(uint8_t i = 0; i < 8; ++i)
		{
			while(HAL_GPIO_ReadPin(DHT11_PIN_GPIO_Port, DHT11_PIN_Pin) == GPIO_PIN_RESET);
			/*
			 * 70 us voltage length means bit=1
			 * 26-28 us voltage length means bit=0
			 *so after e.g 60us if voltage is high it means bit=1
			 *  */
			DWT_Delay_us(60u);
			if(HAL_GPIO_ReadPin(DHT11_PIN_GPIO_Port, DHT11_PIN_Pin) == GPIO_PIN_SET)
			{
				data |= (1 << (7 - i)); 		// bit=1
			}
			else
			{
				data &= ~(1 << (7 - i)); 	// bit=0
			}

			while(HAL_GPIO_ReadPin(DHT11_PIN_GPIO_Port, DHT11_PIN_Pin) == GPIO_PIN_SET);

		}
}
