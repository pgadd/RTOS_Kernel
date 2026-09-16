#include <stdint.h>

#define RCC_AHB2ENR (*(volatile uint32_t *)0x4002104C)

#define GPIOA_MODER (*(volatile uint32_t *)0x48000000)
#define GPIOA_ODR   (*(volatile uint32_t *)0x48000014)

static void delay(void)
{
    for (volatile uint32_t i = 0; i < 1000000; i++)
    {
    }
}

int main(void)
{
    /* Enable GPIOA clock */
    RCC_AHB2ENR |= (1u << 0);

    /* Configure PA5 as a general-purpose output */
    GPIOA_MODER &= ~(3u << 10);
    GPIOA_MODER |=  (1u << 10);

    while (1)
    {
        /* Turn LED on */
        GPIOA_ODR |= (1u << 5);

        delay();

        /* Turn LED off */
        GPIOA_ODR &= ~(1u << 5);

        delay();
    }
}