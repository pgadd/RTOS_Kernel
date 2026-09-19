#include <stdint.h>

#define RCC_AHB2ENR (*(volatile uint32_t *)0x4002104C)

#define GPIOA_MODER (*(volatile uint32_t *)0x48000000)
#define GPIOA_ODR   (*(volatile uint32_t *)0x48000014)

#define SYST_CSR (*(volatile uint32_t *)0xE000E010)
#define SYST_RVR (*(volatile uint32_t *)0xE000E014)

#define MAX_TIMERS 4

volatile uint32_t tick_count = 0;
volatile uint32_t heartbeat = 0;

typedef struct {
    uint32_t next;
    uint32_t period;
    volatile uint32_t expired;
    uint32_t active;
    void (*callback)(void);
} Timer;


//Callback functions
void led_toggle(void) {
    GPIOA_ODR ^= (1u << 5);
}

void heartbeat_update(void) {
    heartbeat++;
}


Timer timers[MAX_TIMERS] = {
    {
        .next = 500,
        .period = 500,
        .expired = 0,
        .active = 1,
        .callback = led_toggle
    },
    {
        .next = 1000,
        .period = 1000,
        .expired = 0,
        .active = 1,
        .callback = heartbeat_update
    }
};


void SysTick_Handler(void)
{
    tick_count++;

    for (int i = 0; i < MAX_TIMERS; i++)
    {
        if (!timers[i].active)
            continue;

        if (tick_count >= timers[i].next)
        {
            timers[i].expired = 1;
            timers[i].next += timers[i].period;
        }
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
        for (int i = 0; i < MAX_TIMERS; i++) {
            if (timers[i].expired) {
                timers[i].expired = 0;

                if (timers[i].callback) {
                    timers[i].callback();
                }
            }
        }

        __asm volatile ("wfi");

    }

}