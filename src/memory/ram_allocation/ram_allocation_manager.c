//
// Created by rfleuryleveso on 17/04/23.
//

#include "ram_allocation_manager.h"
#include "../../print.h"
#include "../../common/os-config.h"

struct RAM_ALLOCATION_MANAGER_ALLOCATION *RAMGetAllocationAtIndex (uint64_t index)
{
  struct RAM_ALLOCATION_MANAGER_ALLOCATION *target_allocation = 0x0;

  struct RAM_ALLOCATION_MANAGER_ALLOCATION *current_allocation = Kmm.ramam_allocations;
  for (uint64_t i = 0; i < index; i++)
	{

	}
}

struct RAM_ALLOCATION_MANAGER_ALLOCATION *
RAMAddAllocation (uint64_t physical_address, uint64_t virtual_address, uint64_t flags)
{

}
void RAMInit ()
{
// Find a large enough free memory
  // Compute the required amount of space
  uint64_t pages_count = (Kmm.available_memory - (Kmm.available_memory % ISENOS_PAGE_SIZE)) / ISENOS_PAGE_SIZE;
  uint64_t ram_allocation_manager_size = (pages_count + 1) * sizeof (struct RAM_ALLOCATION_MANAGER_ALLOCATION);
  printf ("[RAM_ALLOCATION_MANAGER] %d pages will require %d bytes for RAM_ALLOCATION_MANAGER_ALLOCATION \n", pages_count, ram_allocation_manager_size);
  for (uint64_t index = 0; index < Kmm.pmm_ram_range_size; index++)
	{
	  struct PRM_RAM_RANGE *ram_range = &Kmm.prm_ram_ranges[index];
	  uint64_t physical_start = ram_range->start;
	  uint64_t physical_end = ram_range->end;

	  uint64_t range_size = physical_end - physical_start;

	  printf ("[RAM_ALLOCATION_MANAGER] Checking 0x%x - 0x%x (%d bytes) \n", physical_start, physical_end, range_size);

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

	  printf ("[RAM_ALLOCATION_MANAGER] Found a suitable range for RAM Allocation table: 0x%x - 0x%x (%d bytes) \n", physical_start, physical_end, range_size);
	  Kmm.ramam_base = physical_start;
	  Kmm.ramam_top = physical_start + ram_allocation_manager_size;
	  break;
	}
}
