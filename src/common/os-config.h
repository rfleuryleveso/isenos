//
// Created by rfleuryleveso on 18/04/23.
//

#ifndef ISENOS_PRELOADER_OS_CONFIG_H
#define ISENOS_PRELOADER_OS_CONFIG_H

#define ISENOS_PAGE_SIZE 0x200000
#define ISEN_MAPPED_MEMORY_SIZE_GB 16



// #define ISENOS_KERNEL_BASE 0xC0000000
#define ISENOS_KERNEL_BASE 0xFFFF800000000000

#define ISENOS_KERNEL_STACK_BASE (ISENOS_KERNEL_BASE + 0x40000000)
#define ISENOS_KERNEL_STACK_TOP (ISENOS_KERNEL_STACK_BASE + ISENOS_PAGE_SIZE)

#define PAM_BASE (ISENOS_KERNEL_STACK_TOP + 0x40000000)

#define VMT_BASE (PAM_BASE + 0x40000000)
#define VMT_TOP (VMT_BASE + 0x40000000)

#define ACPI_BASE (VMT_TOP)
#define ACPI_TOP (ACPI_BASE + 0x200000)

// Map physical memory to virtual memory
#define PHM_BASE (ACPI_TOP + 0x40000000)
#define PHM_TOP (PHM_BASE + ISEN_MAPPED_MEMORY_SIZE_GB * 0x40000000)

#define VMT_BITMAP_BASE (PHM_TOP)

#define NULL 0

#endif //ISENOS_PRELOADER_OS_CONFIG_H
