/*
 * HumiditySensor.cpp
 *
 *  Created on: 6 lis 2019
 *      Author: Student235325
 */

#include "HumiditySensor.hpp"

HumiditySensor::HumiditySensor() {
    for(auto &it: m_data)
        it = 0;
}

uint16_t HumiditySensor::getDataByIndex(uint8_t index) const {
    if (station::isInRange(index))
        return m_data[index];
    else
        return 0;
}


HAL_StatusTypeDef HumiditySensor::readHumidity() {
    station::rangeFix(station::available_count);

    return HAL_ADC_Start_DMA(&hadc1, (uint32_t*) m_data, station::available_count);
}


float HumiditySensor::getHumidityByIndex(uint8_t index) const {
    //file:///H:/Download/sunrom-318500.pdf
    //TODO: check if pdf describes used sensor
    float humidity = getDataByIndex(index) / 1023.f * 100;

    return humidity;
}
