#ifndef OS_KERNEL_H
#define OS_KERNEL_H

#include <stdint.h>

#define MAX_TASKS 3
#define STACK_SIZE 256 // 256 words = 1024 bytes per stack

typedef enum {
    TASK_READY,
    TASK_BLOCKED,
    TASK_RUNNING
} TaskState_t;

typedef struct {
    uint32_t *sp;         // Stack Pointer
    uint8_t priority;     
    TaskState_t state;    
} TCB_t;


void os_kernel_init(void);

// Returns 1 if successful, 0 if we reached MAX_TASKS
int os_task_create(void (*task_function)(void), uint8_t priority);

#endif