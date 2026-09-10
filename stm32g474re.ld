/* stm32g474re.ld */
ENTRY(Reset_Handler) /* The first assembly instruction to run */

/* Highest address of the user mode stack */
_estack = ORIGIN(RAM) + LENGTH(RAM); /* 0x20000000 + 128K */

MEMORY
{
    /* STM32G474RE Memory Map */
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
    /* The interrupt vector table goes at the very beginning of Flash */
    .isr_vector :
    {
        KEEP(*(.isr_vector))
    } > FLASH

    /* The actual program code and constants */
    .text :
    {
        *(.text)
        *(.rodata)
    } > FLASH

    /* Initialized variables (.data) - stored in Flash, copied to RAM at boot */
    _sidata = LOADADDR(.data);
    .data :
    {
        _sdata = .;
        *(.data)
        _edata = .;
    } > RAM AT> FLASH

    /* Uninitialized variables (.bss) - allocated in RAM and zeroed out at boot */
    .bss :
    {
        _sbss = .;
        *(.bss)
        _ebss = .;
    } > RAM
}