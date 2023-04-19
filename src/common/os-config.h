//
// Created by rfleuryleveso on 18/04/23.
//

#ifndef ISENOS_PRELOADER_OS_CONFIG_H
#define ISENOS_PRELOADER_OS_CONFIG_H

#define ISENOS_PAGE_SIZE 0x200000
#define ISEN_MAPPED_MEMORY_SIZE_GB 128



// #define ISENOS_KERNEL_BASE 0xC0000000
#define ISENOS_KERNEL_BASE 0xFFFF800000000000

#define PAM_BASE (ISENOS_KERNEL_BASE + 0x40000000)

// Map physical memory to virtual memory
#define PHM_BASE (PAM_BASE + 0x40000000)
#define PHM_TOP (PAM_BASE + ISEN_MAPPED_MEMORY_SIZE_GB * 0x40000000)


#define NULL 0

#endif //ISENOS_PRELOADER_OS_CONFIG_H
