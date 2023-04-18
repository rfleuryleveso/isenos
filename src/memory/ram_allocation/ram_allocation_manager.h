//
// Created by rfleuryleveso on 17/04/23.
//

#ifndef ISENOS_PRELOADER_RAM_ALLOCATION_MANAGER_H
#define ISENOS_PRELOADER_RAM_ALLOCATION_MANAGER_H

#include "ram_allocation_manager_allocation.h"
#include "../kernel_memory_information.h"

/**
 * Return an allocation by it's index in the chain
 * @param index
 * @return NULL in case of ERROR, a pointer to the struct if the allocation was found successfuly
 */
struct RAM_ALLOCATION_MANAGER_ALLOCATION* RAMGetAllocationAtIndex(uint64_t index);


struct RAM_ALLOCATION_MANAGER_ALLOCATION* RAMAddAllocation(uint64_t physical_address, uint64_t virtual_address, uint64_t flags);

void RAMInit();

#endif //ISENOS_PRELOADER_RAM_ALLOCATION_MANAGER_H
