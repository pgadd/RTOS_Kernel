#include "os_kernel.h"

// Pointer to keep track of current task
TCB_t *current_tcb;

// Statically allocate the TCBs and the Stacks for all tasks
TCB_t TCB_array[MAX_TASKS];
uint32_t task_stacks[MAX_TASKS][STACK_SIZE];



void os_kernel_init(void) {
    // Clear out the TCB array
    for (int i = 0; i < MAX_TASKS; i++) {
        TCB_array[i].sp = 0;
        TCB_array[i].state = TASK_READY;
        TCB_array[i].priority = 0;
    }
    
    // Initialize our current task pointer to NULL
    current_tcb = 0;
}

// Keep track of how many tasks we've created
static uint8_t task_count = 0;

int os_task_create(void (*task_function)(void), uint8_t priority) {
    if (task_count >= MAX_TASKS) {
        return 0; // Kernel is full
    }
    
    // 1. Get the top of this task's stack 
    // (Arrays grow up in address, but ARM stacks grow down!)
    uint32_t *stack_ptr = &task_stacks[task_count][STACK_SIZE];
    
    // 2. Walk down the stack to make room for the 8 hardware registers
    stack_ptr -= 8; 
    
    // 3. Fill in the critical fake hardware registers
    stack_ptr[7] = 0x01000000;         // xPSR (Set Thumb bit 24 to 1)
    stack_ptr[6] = (uint32_t)task_function; // PC (The task's entry point)
    stack_ptr[5] = 0xFFFFFFFD;         // LR (Magic return value, we'll explain later)
    // R12, R3, R2, R1, R0 can just be 0 (stack_ptr[4] down to stack_ptr[0])
    
    // 4. Save the adjusted stack pointer into the TCB
    TCB_array[task_count].sp = stack_ptr;
    TCB_array[task_count].priority = priority;
    TCB_array[task_count].state = TASK_READY;
    
    task_count++;
    return 1;
}

// Starts the timer to fire an interrupt every 1 millisecond
SysTick_Config(SystemCoreClock / 1000);sd

void SysTick_Handler(void) {
    // 1. We could update system time or unblock delayed tasks here
    
    // 2. Trigger the PendSV exception to handle the actual context switch
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; 
}