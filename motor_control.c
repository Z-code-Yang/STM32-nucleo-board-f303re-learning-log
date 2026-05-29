/*STM32 (stm32f303xe.h) bare metal programming for control step motor from Arduino Kit */
//using the half-step sequence
/*1000  
1100  
0100  
0110  
0010  
0011  
0001  
1001*/

#include <stdint.h>

/* GPIOA registers locations*/
#define GPIOA_BASE         0x48000000UL
#define GPIOA_MODER        (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR          (*(volatile uint32_t *)(GPIOA_BASE + 0x14))
#define GPIOA_BRR          (*(volatile uint32_t *)(GPIOA_BASE + 0x28))
#define GPIOA_IDR   	   (*(volatile uint32_t *)(GPIOA_BASE + 0x10))
#define GPIOA_PUPDR 	   (*(volatile uint32_t *)(GPIOA_BASE + 0x0C))

#define GPIOB_BASE         0x48000400UL
#define GPIOB_MODER        (*(volatile uint32_t *)(GPIOB_BASE + 0x00))
#define GPIOB_ODR          (*(volatile uint32_t *)(GPIOB_BASE + 0x14))
#define GPIOB_BRR          (*(volatile uint32_t *)(GPIOB_BASE + 0x28))


/* RCC registers */
#define RCC_BASE           0x40021000UL
#define RCC_AHBENR        (*(volatile uint32_t *)(RCC_BASE + 0x14))

//motor pins
#define IN1 (1 << 0)  // PA0
#define IN2 (1 << 1)  // PA1
#define IN3 (1 << 4)  // PA4
#define IN4 (1 << 0)  // PB0
//button pin
#define BTN_PIN     (1 << 6)  // PA6

void delay(volatile uint32_t count)
{
    while (count != 0)
    {
    	count--;
        __asm__("nop");
    }
}

int main(void)
{
    RCC_AHBENR = (1 << 17) | (1 << 18);

    // Motor output pins
    GPIOA_MODER |= (1 << 0);   // PA0
    GPIOA_MODER |= (1 << 2);   // PA1
    GPIOA_MODER |= (1 << 8);   // PA4
    GPIOA_MODER |= (1 << 10);  // PA5 (LED)
    GPIOB_MODER |= (1 << 0);   // PB0

    // PA6 as input (MODER bits 13:12 = 00)
    GPIOA_MODER &= ~(3 << 12);
    // Pull-up on PA6
    GPIOA_PUPDR |= (1 << 12);

    int motor_on = 0;

    while (1)
    {
        // Button pressed (active LOW)
    	if ((GPIOA_IDR & BTN_PIN) == 0)          // if PA6 is LOW (button pressed)
    	{
    	    if (motor_on == 0)                    // toggle
    	        motor_on = 1;
    	    else
    	        motor_on = 0;

    	    delay(50000);

    	    while ((GPIOA_IDR & BTN_PIN) == 0);  // loop while PA6 still LOW (held down)

    	    delay(50000);
    	}

        if (motor_on == 1)
        {
            GPIOA_ODR = IN1;            GPIOB_ODR = 0;      delay(5000);
            GPIOA_ODR = IN1 | IN2;      GPIOB_ODR = 0;      delay(5000);
            GPIOA_ODR = IN2;            GPIOB_ODR = 0;      delay(5000);
            GPIOA_ODR = IN2 | IN3;      GPIOB_ODR = 0;      delay(5000);
            GPIOA_ODR = IN3;            GPIOB_ODR = 0;      delay(5000);
            GPIOA_ODR = IN3;            GPIOB_ODR = IN4;    delay(5000);
            GPIOA_ODR = 0;              GPIOB_ODR = IN4;    delay(5000);
            GPIOA_ODR = IN1;            GPIOB_ODR = IN4;    delay(5000);
        }
        else
        {
            GPIOA_ODR = 0;
            GPIOB_ODR = 0;
        }
    }
}
