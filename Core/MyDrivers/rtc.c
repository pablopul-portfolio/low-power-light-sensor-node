/*
 * rtc.c
 *
 *  Created on: Jul 19, 2026
 *      Author: PABLO
 */

#include "stm32l1xx.h"
#include "rtc.h"


void RTC_Init(void){

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;  // Enable clock of PWR for APB1
	PWR->CR |= PWR_CR_DBP;  // Allows to write in backup domain

	// Key to allow writing on RTC
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	RCC->CSR |= RCC_CSR_LSEON;  // Enable LSE

	while((RCC->CSR & RCC_CSR_LSERDY) == 0);  // Wait for LSE to be ready

	RCC->CSR &= ~(3 << 16);
	RCC->CSR |= (1 << 16);  // RTCSEL = 01, LSE is selected


	RCC->CSR |= RCC_CSR_RTCEN;  // RTC clock enabled

	EXTI->IMR |= (1 << 20);
	EXTI->RTSR |= (1 << 20);  // Exti in rising edge


	RTC->CR |= RTC_CR_WUTIE;  // WUTIE = 1, Wakeup timer interrupt

	NVIC->ISER[0] |= (1 << 3);  // Enable RTC_WKUP IRQ

	RTC->WPR = 0xFF;  // Protect config again
}

void RTC_SetWakeup(uint32_t seconds){

	//RTC->CR |= RTC_CR_WUTE;  // Enable wakeup timer
}



