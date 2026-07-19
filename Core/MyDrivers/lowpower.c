/*
 * lowpower.c
 *
 *  Created on: Jul 18, 2026
 *      Author: PABLO
 */

#include "stm32l1xx.h"
#include "lowpower.h"

void LOWPOWER_EnterStopMode(void){

	// PWR clock enabled in rtc.c

	PWR->CR |= PWR_CR_LPSDSR;
	PWR->CR &= ~PWR_CR_PDDS;

	PWR->CSR |= PWR_CSR_WUF;

	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;  // SLEEPDEEP = 1 (Stop mode)

	__WFI();

	//SystemClock_Config();

}
