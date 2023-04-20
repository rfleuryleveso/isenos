//
// Created by rfleuryleveso on 17/04/23.
//

#include "page_allocation_manager.h"

struct PAGE_ALLOCATION_MANAGER_ALLOCATION *pam_get_base ()
{
  return Kmm.memory_setup_complete ? Kmm.pam_allocations
								   : (void *)Kmm.pam_base;
}

struct PAGE_ALLOCATION_MANAGER_ALLOCATION *pam_get_allocation_for_physical_address (uint64_t address)
{
  struct PAGE_ALLOCATION_MANAGER_ALLOCATION *base_allocation = pam_get_base ();
  for (uint64_t i = 0; i < Kmm.pam_allocations_count; i++)
	{
	  struct PAGE_ALLOCATION_MANAGER_ALLOCATION *allocation = &base_allocation[i];
	  // printf ("Searching allocation for physical address  %016"PRIx64" (ALIGNED = %016"PRIx64") : %016"PRIx64" \n", address, (uint64_t)(ALIGN_PTR(address, ISENOS_PAGE_SIZE)), allocation->physical_start);
	  if ((uint64_t)(ALIGN_PTR(address, ISENOS_PAGE_SIZE)) == allocation->physical_start)
		{
		  return allocation;
		}
	}
  return NULL;
}

struct PAGE_ALLOCATION_MANAGER_ALLOCATION *pam_get_allocation_for_virtual_address (uint64_t address)
{
  struct PAGE_ALLOCATION_MANAGER_ALLOCATION *base_allocation = pam_get_base ();
  for (uint64_t i = 0; i < Kmm.pam_allocations_count; i++)
	{
	  struct PAGE_ALLOCATION_MANAGER_ALLOCATION *allocation = &base_allocation[i];
	  if (((uint64_t)(ALIGN_PTR(address, ISENOS_PAGE_SIZE)) == allocation->virtual_start))
		{
		  return allocation;
		}
	}
  return NULL;
}

struct PAGE_ALLOCATION_MANAGER_ALLOCATION *
pam_add_allocation (uint64_t physical_address, uint64_t virtual_address, uint64_t flags)
{
  uint64_t aligned_virtual_address = ALIGN_PTR(virtual_address, ISENOS_PAGE_SIZE);
  // Find the corresponding page structure
  struct PAGE_ALLOCATION_MANAGER_ALLOCATION *base_allocation = pam_get_allocation_for_physical_address (physical_address);
  if (base_allocation == NULL)
	{
	  printf ("[PAGE_ALLOCATION_MANAGER] ERROR ! Could not find page for physical address:  0x%x\n", physical_address);
	  return NULL;
	}
  base_allocation->virtual_start = aligned_virtual_address;
  base_allocation->flags = flags;
  printf ("[PAGE_ALLOCATION_MANAGER] Added allocation for PHYSICAL = 0x%x, VIRTUAL = 0x%x, ALIGNED_VIRTUAL = 0x%x\n", physical_address, virtual_address, aligned_virtual_address);
  return base_allocation;
}

uint64_t pam_find_free_pages (int pages)
{
  struct PAGE_ALLOCATION_MANAGER_ALLOCATION *base_allocation = pam_get_base ();

  struct PAGE_ALLOCATION_MANAGER_ALLOCATION *current_pam_allocation = NULL;
  struct PAGE_ALLOCATION_MANAGER_ALLOCATION *pam_chain_first = NULL;
  struct PAGE_ALLOCATION_MANAGER_ALLOCATION *pam_chain_current = NULL;

  uint64_t current_allocated_pages = 0;
  printf ("FreePageFinder: Looking for %d pages, with pam_allocations_count = %d\n",
		  pages,
		  Kmm.pam_allocations_count
  );
  for (int pam_allocation_index = 0; pam_allocation_index < Kmm.pam_allocations_count; pam_allocation_index++)
	{
	  current_pam_allocation = &base_allocation[pam_allocation_index];
	  printf ("FreePageFinder: Index %05d, current_allocated_pages = %02d, current_pam_allocation-> %016"PRIx64" pam_chain_first = %016"PRIx64" pam_chain_current %016"PRIx64"\n",
			  pam_allocation_index,
			  current_allocated_pages,
			  current_pam_allocation->physical_start,
			  pam_chain_first == NULL ? 0 : pam_chain_first->physical_start,
			  pam_chain_current == NULL ? 0 : pam_chain_current->physical_start
	  );
	  if (current_allocated_pages == pages)
		{
		  printf ("FreePageFinder: FOUND !\n" );
		  break;
		}


	  if ((current_pam_allocation->flags & PAMA_FLAG_PRESENT) == 0)
		{
		  // We found a free page.
		  if (pam_chain_first == NULL)
			{
			  pam_chain_first = current_pam_allocation;
			  pam_chain_current = current_pam_allocation;
			  current_allocated_pages = 1;
			}
		  else
			{
			  // Previous page of the chain + page size equals the start of the current page, we are good !
			  if ((pam_chain_current->physical_start + ISENOS_PAGE_SIZE) == current_pam_allocation->physical_start)
				{
				  pam_chain_current = current_pam_allocation;
				  current_allocated_pages += 1;
				}
			  else
				{
				  pam_chain_first = NULL;
				  pam_chain_current = NULL;
				}

			}
		}
	}

  return pam_chain_first == NULL ? 0 : pam_chain_first->physical_start;
}

void pam_init_find_region ()
{
  // Find a large enough free memory
  // Compute the required amount of space
  uint64_t pages_count = (Kmm.available_memory - (Kmm.available_memory % ISENOS_PAGE_SIZE)) / ISENOS_PAGE_SIZE;
  uint64_t ram_allocation_manager_size = (pages_count + 256) * sizeof (struct PAGE_ALLOCATION_MANAGER_ALLOCATION);
  printf ("[PAGE_ALLOCATION_MANAGER] %d pages will require %d bytes for PAGE_ALLOCATION_MANAGER_ALLOCATION \n", pages_count, ram_allocation_manager_size);
  for (uint64_t index = 0; index < Kmm.prm_ram_range_size; index++)
	{
	  struct PRM_RAM_RANGE *ram_range = &Kmm.prm_ram_ranges[index];
	  printf ("[PAGE_ALLOCATION_MANAGER] Checking 0x%x - 0x%x (%d bytes) \n", ram_range->start, ram_range->end,
			  ram_range->end - ram_range->start);

	  uint64_t physical_start = (uint64_t)ALIGN_PTR_UPWARD(ram_range->start, ISENOS_PAGE_SIZE);
	  uint64_t physical_end = (uint64_t)ALIGN_PTR_DOWNWARD(ram_range->end, ISENOS_PAGE_SIZE);

	  // If region is smaller than one page, then pointer alignement will kill it
	  if (physical_start > physical_end)
		{
		  printf ("> Range is < 1 page\n");
		  continue;
		}

	  uint64_t range_size = physical_end - physical_start;

	  printf ("[PAGE_ALLOCATION_MANAGER] Checking 0x%x - 0x%x (%d bytes) \n", physical_start, physical_end, range_size);

	  // If the RAM range is too small for the allocator, continue to the next range
	  if (range_size < ram_allocation_manager_size)
		continue;

	  // Check that we don't overlap available memory that is already used for other purposes

	  /**
	   * TODO: This algorithm to avoid used memory sections can be vastly improved to find space before / after, if the reserved space is inside the region
	   */
	  // Check if the virtual memory tables are not in the range
	  if (Kmm.virtual_memory_map_base > physical_start && Kmm.virtual_memory_map_base < physical_end)
		{
		  // VMM starts inside with the current available region
		  physical_end = Kmm.virtual_memory_map_base;
		}
	  if (Kmm.virtual_memory_map_top > physical_start && Kmm.virtual_memory_map_top < physical_end)
		{
// VMM starts inside with the current available region
		  physical_start = Kmm.virtual_memory_map_top;
		}

	  // Recheck range size
	  range_size = physical_end - physical_start;
	  // If the RAM range is too small for the allocator, continue to the next range
	  if (range_size < ram_allocation_manager_size)
		continue;

	  printf ("[PAGE_ALLOCATION_MANAGER] Found a suitable range for RAM Allocation table: 0x%x - 0x%x (%d bytes) \n", physical_start, physical_end, range_size);
	  Kmm.pam_base = physical_start;
	  Kmm.pam_top = physical_start + ram_allocation_manager_size;
	  break;
	}
}
void pam_init_structure ()
{
  // Find a large enough free memory
  // Compute the required amount of space
  struct PAGE_ALLOCATION_MANAGER_ALLOCATION *base_allocation = pam_get_base();
  uint64_t pam_allocation_index = 0;

  // For each physical address range
  for (int range_index = 0; range_index < Kmm.prm_ram_range_size; range_index++)
	{
	  struct PRM_RAM_RANGE *range = &Kmm.prm_ram_ranges[range_index];
	  // For each aligned page
	  for (uint64_t address = (uint64_t)ALIGN_PTR_UPWARD(range->start, ISENOS_PAGE_SIZE);
		   address < range->end;
		   address += ISENOS_PAGE_SIZE)
		{
		  // printf ("[PAGE_ALLOCATION_MANAGER] Init structure %4d for PhysicalRamRange #%d, Physical Address: 0x%x \n", pam_allocation_index, range_index, address);
		  struct PAGE_ALLOCATION_MANAGER_ALLOCATION *current_allocation = &base_allocation[pam_allocation_index++];
		  current_allocation->physical_start = address;
		}
	}
  printf ("[PAGE_ALLOCATION_MANAGER] Allocated %d, total = %d bytes\n", pam_allocation_index,
		  pam_allocation_index * ISENOS_PAGE_SIZE);
  Kmm.pam_allocations_count = pam_allocation_index;
}

void pam_init ()
{
  pam_init_find_region ();
  pam_init_structure ();

}
void pam_debug_print ()
{

  struct PAGE_ALLOCATION_MANAGER_ALLOCATION *base_allocation = pam_get_base();

  printf ("[PAGE_ALLOCATION_MANAGER][DEBUG] Printing %d allocations @ 0x%x\n", Kmm.pam_allocations_count, base_allocation);
  // For each pam idt_entries
  for (int pam_allocation_index = 0; pam_allocation_index < Kmm.pam_allocations_count; pam_allocation_index++)
	{
	  struct PAGE_ALLOCATION_MANAGER_ALLOCATION *allocation = &base_allocation[pam_allocation_index];
	  if ((allocation->flags & PAMA_FLAG_PRESENT) == PAMA_FLAG_PRESENT)
		{
		  printf ("	%04d : PHYSICAL_START = %016"PRIx64" VIRTUAL_START = %016"PRIx64" FLAGS = %04"PRIx64" REFERENCES_COUNT = %d\n", pam_allocation_index, allocation->physical_start, allocation->virtual_start, allocation->flags, allocation->references_count);
		}
	}

}
