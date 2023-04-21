//
// Created by rfleuryleveso on 18/04/23.
//

#ifndef ISENOS_PRELOADER_KERNEL_MEMORY_INFORMATION_H
#define ISENOS_PRELOADER_KERNEL_MEMORY_INFORMATION_H

#include <stdbool.h>
#include "physical_ram/physical_ram_range.h"
#include "page_allocation/page_allocation_manager_allocation.h"

#define KMI_PRM_RAM_RANGE_SIZE 512

// Kernel memory information structure
struct KERNEL_MEMORY_INFORMATION {
	// lmao, this is beyond hell. This CAN'T go well
	uint8_t stack[16384];

	// Total available memory in the system
	uint64_t available_memory;

	// Pointer to the first PRM_RAM_RANGE
	// Set from the start, since we are already running in memory mapped mode.
	struct PRM_RAM_RANGE prm_ram_ranges[KMI_PRM_RAM_RANGE_SIZE];

	// Number of entries in the prm_ram_ranges
	uint64_t prm_ram_range_size;

	// Pointer to the table of ram allocations. This is a fixed size
	// ONLY AVAILABLE WITH memory_setup_complete = true
	struct page_allocation_manager_allocation_t *pam_allocations;

	// Number of PAM allocations
	uint64_t pam_allocations_count;

	// Physical addresses of kernel elements
	uint64_t kernel_base;
	uint64_t kernel_top;
	uint64_t virtual_memory_map_base;
	uint64_t virtual_memory_map_top;
	uint64_t pam_base;
	uint64_t pam_top;

	// True, if memory setup has been completed for kernel and all memory is available correctly
	uint8_t memory_setup_complete;

	uint64_t acpi_base;
	uint64_t acpi_top;
};

extern struct KERNEL_MEMORY_INFORMATION Kmm;

#endif //ISENOS_PRELOADER_KERNEL_MEMORY_INFORMATION_H
