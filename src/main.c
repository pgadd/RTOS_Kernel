#include <stdint.h>

#define RCC_AHB2ENR (*(volatile uint32_t *)0x4002104C)

#define GPIOA_MODER (*(volatile uint32_t *)0x48000000)
#define GPIOA_ODR   (*(volatile uint32_t *)0x48000014)

#define SYST_CSR (*(volatile uint32_t *)0xE000E010)
#define SYST_RVR (*(volatile uint32_t *)0xE000E014)

volatile uint32_t tick_count = 0;
volatile uint32_t led_timer_expired = 0;

typedef struct {
    uint32_t next;
    uint32_t period;
} Timer;

Timer led_timer = {
    .next = 500,
    .period = 500
};

void SysTick_Handler(void)
{
    tick_count++;

    if (tick_count >= led_timer.next) {
        led_timer_expired = 1;
        led_timer.next += led_timer.period;
    }
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


    while (1) {
        if (led_timer_expired) {
            led_timer_expired = 0;

            GPIOA_ODR ^= (1u << 5);
        }

        __asm volatile ("wfi");
    }
}