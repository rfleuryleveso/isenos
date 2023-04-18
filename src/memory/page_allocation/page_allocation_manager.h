//
// Created by rfleuryleveso on 17/04/23.
//

#ifndef ISENOS_PRELOADER_RAM_ALLOCATION_MANAGER_H
#define ISENOS_PRELOADER_RAM_ALLOCATION_MANAGER_H

#include "page_allocation_manager_allocation.h"
#include "../kernel_memory_information.h"

/**
 * Return an allocation by the physical address in memory handled by this address
 * @param address
 * @return NULL in case of ERROR, a pointer to the struct if the allocation was found successfuly
 */
struct PAGE_ALLOCATION_MANAGER_ALLOCATION* RAMGetAllocationForPhysicalAddress(uint64_t address);


struct PAGE_ALLOCATION_MANAGER_ALLOCATION* PAMAddAllocation(uint64_t physical_address, uint64_t virtual_address, uint64_t flags);

void PAMInit();

#endif //ISENOS_PRELOADER_RAM_ALLOCATION_MANAGER_H
