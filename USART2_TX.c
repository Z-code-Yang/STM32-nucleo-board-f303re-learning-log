#include <stdint.h>

#define GPIOA   0x48000000UL
#define GPIOA_MODER (*(volatile uint32_t*)(GPIOA + 0x00))
#define GPIOA_AFRL  (*(volatile uint32_t*)(GPIOA + 0x20))

#define RCC   0x40021000UL
#define RCC_AHBENR  (*(volatile uint32_t*)(RCC + 0x14))
#define RCC_APB1ENR (*(volatile uint32_t*)(RCC + 0x1C))


#define USART2 0x40004400UL
#define USART2_BRR (*(volatile uint32_t*)(USART2 + 0x0C)) // baud rate
#define USART2_ISR (*(volatile uint32_t*)(USART2 + 0x1C))  // status flags
#define USART2_TDR (*(volatile uint32_t*)(USART2 + 0x28)) // transmit data
#define USART2_CR1 (*(volatile uint32_t*)(USART2 + 0x00))// enable USART and TX


void delay(volatile uint32_t t)
{
    while(t--);
}

void UART_SendString(char *str)
{
    while(*str)
    {
    	while(!(USART2_ISR & (1U<<7)));
        USART2_TDR = *str;
        str++;
    }

}
int main(){

	RCC_APB1ENR |= (1U << 17); //enable USART2
	RCC_AHBENR |= (1U << 17); //enable GPIOA

	GPIOA_MODER |= (1U << 5); // configure GPIOA PA2 as Alternative function GPIO
	GPIOA_AFRL |= (0x7U << 8); //GPIOA PA2 as AF7 which is USART2 TX

	USART2_BRR = 208U; //Baud Rate ≈ USART Clock / BRR  8Mhz/208 = 38400 baud

	USART2_CR1 |= (1U<<3); //transmitter enable
	USART2_CR1 |= (1U<<0); //USART enable enable

	while(1)
	{
		UART_SendString("Hello World\r\n");
	    delay(500000);

	}


}
