.syntax unified
.cpu cortex-m4
.thumb

.extern current_tcb       /* Bring in our C pointer */
.global PendSV_Handler    /* Make this handler visible to the hardware */

.section .text
PendSV_Handler:
    /* Step 1: Save the remaining registers to the current task's stack */
    push {r4-r11}

    /* Get the memory address of the current_tcb pointer into R0 */
    ldr r0, =current_tcb
    
    /* Load the actual TCB pointer value (address of the struct) into R1 */
    ldr r1, [r0]

    /* Step 2: Save the new Stack Pointer into the current task's TCB */
    str sp, [r1]

    /* Step 3: Call the C scheduler to pick the next task */
    bl os_scheduler