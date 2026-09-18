#include <stdint.h>

#define RCC_AHB2ENR (*(volatile uint32_t *)0x4002104C)

#define GPIOA_MODER (*(volatile uint32_t *)0x48000000)
#define GPIOA_ODR   (*(volatile uint32_t *)0x48000014)

#define SYST_CSR (*(volatile uint32_t *)0xE000E010)
#define SYST_RVR (*(volatile uint32_t *)0xE000E014)

volatile uint32_t tick_count = 0;

void SysTick_Handler(void)
{
    tick_count++;
}

int main(void)
{
    /* Enable GPIOA clock */
    RCC_AHB2ENR |= (1u << 0);

    /* Configure PA5 as a general-purpose output */
    GPIOA_MODER &= ~(3u << 10);
    GPIOA_MODER |=  (1u << 10);

     /* Configure SysTick */
    SYST_RVR = 15999;
    SYST_CSR = (1u << 0) |   /* ENABLE */
               (1u << 1) |   /* TICKINT */
               (1u << 2);    /* CLKSOURCE */

    uint32_t last_tick = 0;

    while (1)
    {
        if ((tick_count - last_tick) >= 100)
        {
            last_tick = tick_count;

            GPIOA_ODR ^= (1u << 5);
        }
    }
}