.syntax unified
.cpu cortex-m4
.thumb

.global Reset_Handler
.global _estack

.section .isr_vector, "a", %progbits
.type g_pfnVectors, %object

g_pfnVectors:
    .word _estack
    .word Reset_Handler

.size g_pfnVectors, . - g_pfnVectors


.section .text.Reset_Handler
.type Reset_Handler, %function

Reset_Handler:

    /* Copy .data from Flash to RAM */
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata

copy_data:
    cmp r1, r2
    bcc copy_data_word
    b zero_bss

copy_data_word:
    ldr r3, [r0]
    str r3, [r1]

    adds r0, r0, #4
    adds r1, r1, #4

    b copy_data


zero_bss:

    ldr r1, =_sbss
    ldr r2, =_ebss
    movs r3, #0

zero_bss_loop:
    cmp r1, r2
    bcc zero_bss_word
    b call_main

zero_bss_word:
    str r3, [r1]

    adds r1, r1, #4

    b zero_bss_loop


call_main:

    bl main

hang:
    b hang

.size Reset_Handler, . - Reset_Handler