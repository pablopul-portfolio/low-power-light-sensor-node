/*
 * my_uart.h
 *
 *  Created on: Jul 12, 2026
 *      Author: PABLO
 */

#include <string.h>
#include <stdio.h>


void MY_UART_Init(void);

void MY_UART_SendChar(char c);

void MY_UART_SendString(const char *str);

void MY_UART_SendLux(float lux_value);


