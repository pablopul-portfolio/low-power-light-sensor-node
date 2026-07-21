/*
 * lowpower.c
 *
 *  Created on: Jul 18, 2026
 *      Author: PABLO
 */

#include "stm32l1xx.h"
#include "lowpower.h"

extern void SystemClock_Config(void);


void LOWPOWER_EnterStopMode(void){

	// PWR clock enabled in rtc.c

	PWR->CR |= PWR_CR_DBP;  // Backup domain

	//DBGMCU->CR |= DBGMCU_CR_DBG_STOP;    // This was added due to issues connecting to debugger

	PWR->CR |= PWR_CR_LPSDSR;
	PWR->CR &= ~PWR_CR_PDDS;

	PWR->CR |= PWR_CR_CWUF;  // Clear Wakeup flag

	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;  // SLEEPDEEP = 1 (Stop mode)

	HAL_SuspendTick();  // Some interrupts may occur here and make it not enter the stop mode

	__DSB();
	__WFI();

	HAL_ResumeTick();

	SystemClock_Config();

}
