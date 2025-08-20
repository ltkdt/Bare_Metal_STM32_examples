#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"

// Enable flag for UART: R - Receiver, T - Transmitter, U - USART, E - enable
#define CR1_RE  (1U<<2)
#define CR1_TE	(1U<<3)
#define CR1_UE	(1U<<13)

// Transmit / Read data register empty flag
#define SR_TXE	(1U<<7)
#define SR_RXNE (1U<<5)

#define SYS_FREQ		16000000
#define APB1_CLK		SYS_FREQ

#define UART_BAUDRATE		115200

void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk,  uint32_t BaudRate);

// Initialization of UART module 
void uart2_init(void)
{
	// Enable clock access to GPIOA
	RCC->AHB1ENR |= (1U<<0);

	// Set pin A2 to use alternate function mode
	GPIOA->MODER |= (0b10 << 4);

	// Set PA2 alternate function type to UART_TX (AF7)
	GPIOA->AFR[0] |= (0b0111 << 8);

	// Set pin A3 to use alternate function mode
	GPIOA->MODER |= (0b10 << 6);
	// Set PA2 alternate function type to UART_RX (AF7)
	GPIOA->AFR[0] |= (0b0111 << 12);

	// Enable clock access to uart2 
	RCC->APB1ENR |= (1U<<17);

	// Configure baudrate
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);

	// Configure the transfer direction: both transmission and receiver. If only use transmission mode, keep TE only and vice versa 
	USART2->CR1 = (CR1_TE | CR1_RE);

	/*Enable uart module*/
	USART2->CR1 |= CR1_UE;
}

void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk,  uint32_t BaudRate)
{
	// BRR = clock frequency / BaudRate
	uint16_t brr_value = ((PeriphClk + (BaudRate/2U))/BaudRate);

	//  (BaudRate/2U))/BaudRate) simplifies to 1/2 and increase accuracy when rounding in this case 
	USARTx->BRR = brr_value;
}

void uart2_write(int ch)
{
  // Wait for transmitting data register is empty or TXE = 1 (Data is transferred to the shift register)
	while(!(USART2->SR & SR_TXE)){}

  // Write to transmit data register. Note that data register is used in both transmission and reception process 
	USART2->DR	=  (ch & 0xFF);
}

int uart2_read(){
	// Make sure data register is not empty or RXNE = 1 (Received data is ready to be read)
	while(!(USART2->SR & SR_RXNE)) {}

	// Return value of data register
	return USART2->DR;
}

// retarget printf
int __io_putchar(int ch){
	uart2_write(ch);
	return ch;
}

// Retarget scanf
int __io_getchar(void){
	uint8_t ch = uart2_read();
	// Echo on the console
	uart2_write(ch);
	return ch;
}


int main(void)
{
	uart2_init();
	char message[40];
	while(1)
	{
		scanf("%s", message);
		printf("%s", message);
	}
}

