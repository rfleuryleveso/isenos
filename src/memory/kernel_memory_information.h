//
// Created by rfleuryleveso on 18/04/23.
//

#ifndef ISENOS_PRELOADER_KERNEL_MEMORY_INFORMATION_H
#define ISENOS_PRELOADER_KERNEL_MEMORY_INFORMATION_H

#include "physical_ram/physical_ram_range.h"
#include "ram_allocation/ram_allocation_manager_allocation.h"

#define KMI_PRM_RAM_RANGE_SIZE 512

// Kernel memory information structure
struct KERNEL_MEMORY_INFORMATION {
    // Total available memory in the system
    uint64_t available_memory;

    // Pointer to the first PRM_RAM_RANGE
    struct PRM_RAM_RANGE prm_ram_ranges[KMI_PRM_RAM_RANGE_SIZE];

    // Number of entries in the PMM_RAM_RANGE
    uint64_t pmm_ram_range_size;

    // Pointer to the table of ram allocations. This is a fixed size
	// This is not set until the virtual memory maps are generated for the first time !
    struct RAM_ALLOCATION_MANAGER_ALLOCATION* ramam_allocations;


	// Physical addresses of kernel elements
	uint64_t kernel_base;
	uint64_t kernel_top;
	uint64_t virtual_memory_map_base;
	uint64_t virtual_memory_map_top;
	uint64_t ramam_base;
	uint64_t ramam_top;
};

extern struct KERNEL_MEMORY_INFORMATION Kmm;

#endif //ISENOS_PRELOADER_KERNEL_MEMORY_INFORMATION_H
