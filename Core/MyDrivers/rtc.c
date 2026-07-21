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

	RTC->ISR |= RTC_ISR_INIT;
	while (!(RTC->ISR & RTC_ISR_INITF));

	// Prescaler
	RTC->PRER |= (0x7FU << 16);  //PREDIV_A = RTC_clk (32768Hz)
	RTC->PRER |= 0x00FF;  // PREDIV_S = 1, ck_spre frequency = 1 Hz

	RTC->ISR &= ~RTC_ISR_INIT;

	EXTI->PR |= (1 << 20);  // Clear flag
	EXTI->IMR |= (1 << 20);
	EXTI->RTSR |= (1 << 20);  // Exti in rising edge

	RTC->ISR &= ~RTC_ISR_WUTF;  // Wakeup timer flag cleared
	RTC->CR |= RTC_CR_WUTIE;  // WUTIE = 1, Wakeup timer interrupt

	NVIC->ISER[0] |= (1 << 3);  // Enable RTC_WKUP IRQ

	RTC->WPR = 0xFF;  // Protect config again
}

void RTC_SetWakeup(uint32_t seconds){

	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	RTC->CR &= ~RTC_CR_WUTE;

	while((RTC->ISR & RTC_ISR_WUTWF) == 0);  // Polling to wakeup autoreload counter

	RTC->CR &= ~(0x7 << 0);
	RTC->CR |= (1 << 2);   // WUCKSEL = 10x, where: 1 tick = 1 second

	if(seconds == 0){
		seconds = 1;
	}
	RTC->WUTR = (seconds - 1);  // Counts backwards from seconds - 1

	RTC->ISR &= ~RTC_ISR_WUTF;  // Clear previous flag

	RTC->CR |= RTC_CR_WUTE;

	RTC->WPR = 0xFF;
}

void RTC_WKUP_IRQHandler(void){

	if (RTC->ISR & RTC_ISR_WUTF){

		RTC->WPR = 0xCA;
		RTC->WPR = 0x53;

		//GPIOA ->BSRR = (1<<1);

		//GPIOA->ODR ^= (1 << 1);

		RTC->ISR &= ~RTC_ISR_WUTF;  // Clear flag

		EXTI->PR = EXTI_PR_PR20;  // Clear exti

		RTC->WPR = 0xFF;
	}
}



