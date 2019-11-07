/*
 * HumiditySensor.hpp
 *
 *  Created on: 6 lis 2019
 *      Author: Student235325
 */

#ifndef HUMIDITYSENSOR_HPP_
#define HUMIDITYSENSOR_HPP_

#include "watering_common.hpp"

extern ADC_HandleTypeDef hadc1;

class HumiditySensor {
    uint16_t m_data[MAX_STATION_COUNT];

public:
    HumiditySensor();

    uint16_t getDataByIndex(uint8_t index) const;
    HAL_StatusTypeDef readHumidity();

    float getHumidityByIndex(uint8_t index) const;

} hum_sensor;




#endif /* HUMIDITYSENSOR_HPP_ */
