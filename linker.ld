OUTPUT_FORMAT("elf64-x86-64")
ENTRY(main)

SECTIONS
{
    . = 0xC0000000;
    _kernel_start = .;
    .text ALIGN (4K) : AT (ADDR (.text))
    {
        *(.text)
    }
    .rodata ALIGN (4K) : AT (ADDR (.rodata))
    {
        *(.rodata)
    }
    .data ALIGN (4K) : AT (ADDR (.data))
    {
        *(.data)
    }
    .bss ALIGN (4K) : AT (ADDR (.bss))
    {
        *(COMMON)
        *(.bss)
        *(.bootstrap_stack)
    }
    /* Add a symbol that indicates the end address of the kernel. */
    _kernel_end = .;
}