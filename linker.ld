OUTPUT_FORMAT("elf64-x86-64")
ENTRY(main)

SECTIONS
{
    . = 0xFFFF800000000000;
    _kernel_start = .;
    .text ALIGN (2M) : AT (ADDR (.text))
    {
        *(.text)
    }
    .rodata ALIGN (2M) : AT (ADDR (.rodata))
    {
        *(.rodata)
    }
    .data ALIGN (2M) : AT (ADDR (.data))
    {
        *(.data)
    }
    .bss ALIGN (2M) : AT (ADDR (.bss))
    {
        *(COMMON)
        *(.bss)
        *(.bootstrap_stack)
    }
    /* Add a symbol that indicates the end address of the kernel. */
    _kernel_end = .;
}