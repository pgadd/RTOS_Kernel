#include <stdint.h>

#define RCC_AHB2ENR (*(volatile uint32_t *)0x4002104C)

#define GPIOA_MODER (*(volatile uint32_t *)0x48000000)
#define GPIOA_ODR   (*(volatile uint32_t *)0x48000014)

#define SYST_CSR (*(volatile uint32_t *)0xE000E010)
#define SYST_RVR (*(volatile uint32_t *)0xE000E014)

#define TASK_STACK_SIZE 128

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

typedef struct {
    void (*function)(void);
    uint32_t *stack_pointer;
    uint32_t state;
} TCB;


//Callback functions
void led_task(void)
{
    while (1)
    {
        GPIOA_ODR ^= (1u << 5);
        task_yield();
    }
}

void heartbeat_task(void){
    while (1)
    {
        heartbeat++;
        task_yield();
    }
}

void task_init(TCB *tcb, uint32_t *stack) {
    uint32_t *sp = &stack[TASK_STACK_SIZE];

    *(--sp) = 0x01000000;          // xPSR
    *(--sp) = ((uint32_t)tcb->function); // PC
    *(--sp) = 0xFFFFFFFD;          // LR
    *(--sp) = 0;                   // R12
    *(--sp) = 0;                   // R3
    *(--sp) = 0;                   // R2
    *(--sp) = 0;                   // R1
    *(--sp) = 0;                   // R0

    tcb->stack_pointer = sp;
}

uint32_t led_task_stack[TASK_STACK_SIZE];
uint32_t heartbeat_task_stack[TASK_STACK_SIZE];


TCB led_task_tcb = {
    .function = led_task,
    .stack_pointer = &led_task_stack[TASK_STACK_SIZE],
    .state = 0
};

TCB heartbeat_task_tcb = {
    .function = heartbeat_task,
    .stack_pointer = &heartbeat_task_stack[TASK_STACK_SIZE],
    .state = 0
};



Timer timers[MAX_TIMERS] = {
    {
        .next = 500,
        .period = 500,
        .expired = 0,
        .active = 1,
        .callback = led_task
    },
    {
        .next = 1000,
        .period = 1000,
        .expired = 0,
        .active = 1,
        .callback = heartbeat_task
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

    
    task_init(&led_task_tcb, led_task_stack);
    task_init(&heartbeat_task_tcb, heartbeat_task_stack);   


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