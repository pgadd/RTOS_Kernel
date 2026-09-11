.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

.global g_pfnVectors
.global Reset_Handler

/* Reset Handler: The first code executed on boot */
.section .text.Reset_Handler
Reset_Handler:
    /* 1. Copy .data section from Flash to RAM (omitted for minimal MVP, assuming no initialized globals yet) */
    /* 2. Zero out .bss section (omitted for minimal MVP) */
    
    /* Jump to main application */
    bl main
    
    /* Infinite loop if main ever returns */
    b .


/* Vector Table */
.section .isr_vector,"a",%progbits
g_pfnVectors:
    .word _estack            /* Top of Stack */
    .word Reset_Handler      /* Reset Handler */
    /* (Other exception handlers like NMI, HardFault would go here) */