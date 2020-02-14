#ifndef DWT_STM32_DELAY_H
#define DWT_STM32_DELAY_H

#ifdef __cplusplus
extern "C" {
#endif


//#include "stm32f1xx_hal.h"
#include "stm32wbxx_hal.h"

#ifndef uint32_t
#include <stdint.h>
#endif


uint8_t DWT_COUNTER_ENABLE(void);

void DWT_Reset(void);

void DWT_Delay_us(uint32_t us); // microseconds




#ifdef __cplusplus
}
#endif

#endif
