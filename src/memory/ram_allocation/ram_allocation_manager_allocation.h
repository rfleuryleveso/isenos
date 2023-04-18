//
// Created by rfleuryleveso on 18/04/23.
//

#ifndef ISENOS_PRELOADER_RAM_ALLOCATION_ALLOCATION_H
#define ISENOS_PRELOADER_RAM_ALLOCATION_ALLOCATION_H

#include "stdint.h"

#define RAMA_FLAG_RAMANAGER 0x4
#define RAMA_FLAG_PML4 0x2
#define RAMA_FLAG_KERNEL 0x1
#define RAMA_FLAG_USERSPACE 0x0

/**
 * Represents one allocation of exactly one page of size (ISENOS_PAGE_SIZE) to a program / kernel
 */
struct RAM_ALLOCATION_MANAGER_ALLOCATION {
    uint8_t Flags; // RAMA_FLAG_*
    uint64_t PhysicalStart; // Physical address of the allocation
    uint64_t VirtualStart; // Virtual address
    uint64_t ReferencesCount; // References counter, once it reaches 0, the pages can be reused for another program
    uint32_t ProgramID; // ProgramID for the allocation.
};

#endif //ISENOS_PRELOADER_RAM_ALLOCATION_ALLOCATION_H
