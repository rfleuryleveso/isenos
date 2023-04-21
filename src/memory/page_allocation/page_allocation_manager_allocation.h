//
// Created by rfleuryleveso on 18/04/23.
//

#ifndef ISENOS_PRELOADER_RAM_ALLOCATION_ALLOCATION_H
#define ISENOS_PRELOADER_RAM_ALLOCATION_ALLOCATION_H

#include "stdint.h"

#define PAMA_FLAG_HEAP 0x40
#define PAMA_FLAG_DO_NOT_INCLUDE_IN_VMT 0x20
#define PAMA_FLAG_RAMANAGER 0x10
#define PAMA_FLAG_VMT 0x8
#define PAMA_FLAG_KERNEL 0x4
#define PAMA_FLAG_USERSPACE 0x2
#define PAMA_FLAG_PRESENT 0x1

/**
 * Represents one allocation of exactly one page of size (ISENOS_PAGE_SIZE) to a program / kernel
 */
typedef struct  {
	uint8_t flags; // PAMA_FLAG_*
	uint64_t physical_start; // Physical address of the allocation. !! PAGE ALIGNED !!
	uint64_t virtual_start; // Virtual address. !! PAGE ALIGNED !!
	uint64_t references_count; // References counter, once it reaches 0, the pages can be reused for another program
	uint32_t program_id; // program_id for the allocation.
} page_allocation_manager_allocation_t;

#endif //ISENOS_PRELOADER_RAM_ALLOCATION_ALLOCATION_H
