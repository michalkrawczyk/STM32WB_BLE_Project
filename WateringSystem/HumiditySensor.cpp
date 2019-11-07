/*
 * HumiditySensor.cpp
 *
 *  Created on: 6 lis 2019
 *      Author: Student235325
 */

#include "HumiditySensor.hpp"

HumiditySensor::HumiditySensor() {
    for(auto &it: data)
        it = 0;
}

uint16_t HumiditySensor::getDataByIndex(uint8_t index) const {
    if (station::isInRange(index))
        return data[index];
    else
        return 0;
}


HAL_StatusTypeDef HumiditySensor::readHumidity() {
    station::rangeFix(station::available_count);

    return HAL_ADC_Start_DMA(&hadc1, (uint32_t*) data, station::available_count);
}
