/*
 * dht.h
 *
 *  Created on: 27 lis 2019
 *      Author: Student235325
 */

#ifndef DHT_H_
#define DHT_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef uint8_t
#include "stdint.h"
#endif

#ifndef bool
#include "stdbool.h"
#endif

typedef struct
{
	uint8_t humidity_integral,
			humidity_decimal,
			temperature_integral,
			temperature_decimal,
			parity_bit;
//			timer_id;
} DHT11_t;

extern DHT11_t dht;


bool makeMeasureDHT(void);
void initDHT(void);
uint16_t getTemperature(void);
uint16_t getHumidity(void);

#ifdef __cplusplus
}
#endif

#endif /* DHT_H_ */
