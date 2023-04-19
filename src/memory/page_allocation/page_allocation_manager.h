//
// Created by rfleuryleveso on 17/04/23.
//

#ifndef ISENOS_PRELOADER_RAM_ALLOCATION_MANAGER_H
#define ISENOS_PRELOADER_RAM_ALLOCATION_MANAGER_H

#include "page_allocation_manager_allocation.h"
#include "../kernel_memory_information.h"
#include "../../print.h"
#include "../../common/os-config.h"
#include "../../macros/macros.h"
#include "inttypes.h"

/**
 * Return an allocation by the physical address in memory handled by this address
 * @param address
 * @return NULL in case of ERROR, a pointer to the struct if the allocation was found successfuly
 */
struct PAGE_ALLOCATION_MANAGER_ALLOCATION* PAMGetAllocationForPhysicalAddress(uint64_t address);

/**
 * Return an allocation by the virtual address in memory handled by this address
 * @param address
 * @return NULL in case of ERROR, a pointer to the struct if the allocation was found successfuly
 */
struct PAGE_ALLOCATION_MANAGER_ALLOCATION *PAMGetAllocationForVirtualAddress (uint64_t address);

/**
 * Add a new allocation to th
 * @param physical_address
 * @param virtual_address
 * @param flags
 * @return
 */
struct PAGE_ALLOCATION_MANAGER_ALLOCATION* PAMAddAllocation(uint64_t physical_address, uint64_t virtual_address, uint64_t flags);

/**
 * Initialize the page allocation manager, must be called early in the kernel initialization
 */
void PAMInit();

/**
 * Find contiguous pages in physical memory
 * @param pages
 * @return The physical address of the free pages
 */
uint64_t PAMFindFreePages (int pages);

/**
 * Prints all pages to serial
 */
void PAMDebugPrint();

#endif //ISENOS_PRELOADER_RAM_ALLOCATION_MANAGER_H
