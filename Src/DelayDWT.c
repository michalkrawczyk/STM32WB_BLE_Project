/*
 * DelayDWT.c
 *
 *  Created on: 24 paź 2019
 *      Author: Student235325
 */

#include "DelayDWT.h"


void DWT_Init(void)
{
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;

        //DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; // Clear DWT - additional
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; //Set DWT
    }
}


void DWT_Delay(uint32_t us)
{
    uint32_t startTick = DWT->CYCCNT,
             delayTicks = us * (SystemCoreClock/1000000);

    while (DWT->CYCCNT - startTick < delayTicks);
}



