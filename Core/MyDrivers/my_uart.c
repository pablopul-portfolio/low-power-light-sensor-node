

#include "my_uart.h"
#include "stm32l1xx.h"



//#define RCC_APB1ENR_USART2EN 17

void MY_UART_Init(void){

	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;  // Enable USART2 clock

	USART2->BRR = 0x0D05;  // CLK at 32MHz, with baud rate of 9600

	USART2->CR1 &= ~((1 << 10) | (1 << 12));  //Word length = 8bits, No parity
	USART2->CR1 |=(1 << 3);  // TE = 1 Enable transmitter

	USART2->CR2 = 0x0;  // Stop bit = 1

	USART2->CR1 |= (1 << 13); // Enable

}

void MY_UART_SendChar(char c){

	while (!(USART2->SR & (1 << 7)));  // Wait for TX to be empty

	USART2->DR = (c & 0xFF);
}

void MY_UART_SendString(const char *str){

	while(*str){
		MY_UART_SendChar(*str);
		str++;
	}
}

void MY_UART_SendLux(float lux_value){

	char tx_buffer[30];

	int whole_part = (int)lux_value;
	int decimal_part = (int)((lux_value - whole_part) * 100);

	if (decimal_part < 0) {
	  decimal_part = -decimal_part;
	}

	snprintf(tx_buffer, sizeof(tx_buffer), "Lux: %d.%02d\r\n", whole_part, decimal_part);

	MY_UART_SendString(tx_buffer);
}



