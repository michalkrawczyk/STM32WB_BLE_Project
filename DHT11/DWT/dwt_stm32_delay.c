#include "dwt_stm32_delay.h"

/* TICKS_TO_1us
 * At 4MHz SystemCoreClock one tick takes 0.25 us
 */
#define TICKS_TO_1us 4

/**
 * @brief  Initializes DWT_Clock_Cycle_Count for DWT_Delay_us function
 * @return Error DWT counter
 *         1: clock cycle counter not started
 *         0: clock cycle counter works
 */
uint8_t DWT_COUNTER_ENABLE(void)
{
  uint32_t c;

  CoreDebug->DEMCR &= ~0x01000000;
  CoreDebug->DEMCR |=  0x01000000;

  DWT->CTRL &= ~0x00000001;
  DWT->CTRL |=  0x00000001;

  DWT->CYCCNT = 0;

  c = DWT->CYCCNT;

  //Some operations to check if works
  __ASM volatile ("NOP");
  __ASM volatile ("NOP");
  __ASM volatile ("NOP");

  if((DWT->CYCCNT - c) == 0)
  {
	  return 0;
  }

  return (DWT->CYCCNT - c);
}


void DWT_Reset(void)
{
	DWT->CYCCNT = 0;

	DWT->CTRL &= ~0x00000001;
	DWT->CTRL |=  0x00000001;
}




void DWT_Delay_us(uint32_t us) // microseconds
{
	uint32_t startTick = DWT->CYCCNT;
	uint32_t targetTick = startTick + us * TICKS_TO_1us;


    // Must check if target tick is out of bounds and overflowed
    if (targetTick > startTick) {
        // Not overflowed
        while (DWT->CYCCNT < targetTick);
    } else {
        // Overflowed
        while (DWT->CYCCNT > startTick || DWT->CYCCNT < targetTick);
    }
}
