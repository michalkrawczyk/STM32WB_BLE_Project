/*
 * dwt.h
 *
 *  Created on: 26 lis 2019
 *      Author: Student235325
 */


#ifndef DWT_H_
#define DWT_H_

#define delayUS_ASM(us) do {\
	asm volatile (	"MOV R0,%[loops]\n\t"\
			"1: \n\t"\
			"SUB R0, #1\n\t"\
			"CMP R0, #0\n\t"\
			"BNE 1b \n\t" : : [loops] "r" (16*us) : "memory"\
		      );\
} while(0)

//extern uint32_t SystemCoreClock;
//
//static void initDWT(void)
//{
//	if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
//	        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
//	        DWT->CYCCNT = 0;
//
//	        DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; // Clear DWT - additional
//	        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; //Set DWT
//	    }
//}
//
//static void delayDWT(uint32_t us)
//{
//    uint32_t startTick = DWT->CYCCNT,
//             delayTicks = us * (SystemCoreClock/1000000L);
//
//    uint32_t test = DWT->CYCCNT;
//
//
//    while ((DWT->CYCCNT - startTick) < delayTicks);
//}

#endif /* DELAYDWT_H_ */
