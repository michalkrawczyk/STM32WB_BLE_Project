/*
 * watering_common.hpp
 *
 *  Created on: 7 lis 2019
 *      Author: Student235325
 */

#ifndef WATERING_COMMON_HPP_
#define WATERING_COMMON_HPP_

#ifndef MAX_STATION_COUNT
    #define MAX_STATION_COUNT 10
#endif


namespace station
{
    uint8_t available_count(0);

    bool isInRange(const uint8_t &value) {return value < MAX_STATION_COUNT; };
    void rangeFix(uint8_t &value)
    {
        if(value > MAX_STATION_COUNT)
            value = MAX_STATION_COUNT;
    }
}



#endif /* WATERING_COMMON_HPP_ */
