//
// Created by rfleuryleveso on 17/04/23.
//

#ifndef ISENOS_PRELOADER_RAM_ALLOCATION_MANAGER_H
#define ISENOS_PRELOADER_RAM_ALLOCATION_MANAGER_H

#include "page_allocation_manager_allocation.h"
#include "../kernel_memory_information.h"
#include "../../print.h"
#include "../../common/os-config.h"
#include "../../common/os-types.h"
#include "../../common/ports.h"
#include "inttypes.h"

/**
 * Return an allocation by the physical address in memory handled by this address
 * @param address
 * @return NULL in case of ERROR, a pointer to the struct if the allocation was found successfuly
 */
page_allocation_manager_allocation_t *pam_get_allocation_for_physical_address (uint64_t address);

/**
 * Return an allocation by the virtual address in memory handled by this address
 * @param address
 * @return NULL in case of ERROR, a pointer to the struct if the allocation was found successfuly
 */
page_allocation_manager_allocation_t *pam_get_allocation_for_virtual_address (uint64_t address);

/**
 * Add a new allocation to th
 * @param physical_address
 * @param virtual_address
 * @param flags
 * @return
 */
page_allocation_manager_allocation_t *
pam_add_allocation (uint64_t physical_address, uint64_t virtual_address, uint64_t flags);

/**
 * Initialize the page allocation manager, must be called early in the kernel initialization
 */
void pam_init ();

/**
 * Find contiguous pages in physical memory
 * @param pages
 * @return The physical address of the free pages
 */
uint64_t pam_find_free_pages (int pages);

/**
 * Prints all pages to serial
 */
void pam_debug_print ();

/**
 * Returns the index 0 ptr of the PAM allocations array
 * @return
 */
page_allocation_manager_allocation_t *pam_get_base ();

#endif //ISENOS_PRELOADER_RAM_ALLOCATION_MANAGER_H
