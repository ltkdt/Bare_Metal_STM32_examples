/* main.c file is the first example whereas main_struct.c is the latter part of the video */

#include <stdint.h>

/* PERIPHERAL BASE */

#define PERIPH_BASE (0x40000000UL)

/* OFFSET ADDRESS */

#define AHB1_OFFSET (0x00020000UL)

#define RCC_OFFSET (0x3800UL)

#define AHB1ENR_OFFSET (0x30UL)

#define GPIOB_OFFSET (0x00000400UL)

#define MODER_OFFSET (0x00UL)

#define ODR_OFFSET (0x14UL)

/* BASE ADDRESS */

#define AHB1_BASE     (PERIPH_BASE + AHB1_OFFSET)

#define RCC_BASE    (AHB1_BASE + RCC_OFFSET)

#define GPIOB_BASE (AHB1_BASE + GPIOB_OFFSET)

/* REGISTERS */
#define RCC_ABHEN1R (*(volatile unsigned int *)(RCC_BASE + AHB1ENR_OFFSET))

#define GPIOB_MODER (*(volatile unsigned int *)(GPIOB_BASE + MODER_OFFSET))

#define GPIOB_ODR (*(volatile unsigned int *)(GPIOB_BASE + ODR_OFFSET))


int main(void)
{
    /* ENABLE CLOCK ACCESS TO GPIO PORT B*/
    RCC_ABHEN1R |= (1U<<1); // turn on bit 1

    /* SET PB 5 AS GPIO OUTPUT */

    GPIOB_MODER |= (1U<<10);
    GPIOB_MODER &= ~(1U<<11); // turn off bit 11
    
    while(1){
        /* SET PIN 5 AS HIGH AND LOW REPEATEDLY */
        GPIOB_ODR ^= (1U<<5);

        /* Small note
        set pin 5 as high: GPIOB_ODR ^= (1U<<5);
        set pin 5 as low: GPIOB_ODR &= ~(1U<<5);
        */

        /* DELAY */
        for(int i = 0; i < 1E6; i++) {}
    }
    
}