#include "inttypes.h"
#include "stdint.h"
#include "isenos.h"
#include "print.h"
#include "memory/efi_memory_types.h"
#include "memory/kernel_memory_information.h"
#include "memory/physical_ram/physical_ram_manager.h"
#include "memory/page_allocation/page_allocation_manager.h"
#include "common/os-config.h"
#include "memory/page_allocation/page_allocation_manager_allocation.h"
#include "common/math.h"
#include "memory/virtual_memory_tables/virtual_memory_tables_manager.h"
#include "memory/virtual_memory_tables/static_mappings.h"

uint64_t main (IBL_ISENOS_DATA *IsenOSEntrypointData)
{
  uint8_t ret;
  ret = print_initialize_com1 ();
  printf (ISEN_OS_SPLASH);
  printf ("Initializing ISENOS: MemoryMap pointer %p MemoryMap size %d \n", IsenOSEntrypointData->MemoryMap, IsenOSEntrypointData->MemoryMapSize);

  long long total_usable_ram = 0;
  for (uint32_t index = 0; index < IsenOSEntrypointData->MemoryMapSize; index++)
	{
	  IBL_MEMORY_MAP_ENTRY map_entry = IsenOSEntrypointData->MemoryMap[index];
	  printf ("    Entry %3d | TYPE = %x (", index, map_entry.Type);
	  switch (map_entry.Type)
		{
	  case EfiConventionalMemory:
		printf ("Free RAM            ");
		  total_usable_ram += map_entry.Size;
		  PRMAddRange (map_entry.PhysicalStart, map_entry.PhysicalStart + map_entry.Size);
		  break;
	  case EfiACPIReclaimMemory:
		printf ("ACPI Tables         ");
		  break;
	  case EfiMemoryMappedIO:
		printf ("EfiMemoryMappedIO   ");
		  break;
		}

	  printf (") FROM = %16x TO = %16x, SIZE = %x\n", map_entry.PhysicalStart,
			  map_entry.PhysicalStart + map_entry.Size, map_entry.Size);
	}
  printf ("Total usable RAM = %"PRId64" bytes\n", total_usable_ram);
  Kmm.available_memory = total_usable_ram;
  Kmm.kernel_base = IsenOSEntrypointData->KernelStart;
  Kmm.kernel_top = IsenOSEntrypointData->KernelEnd;
  Kmm.virtual_memory_map_base = IsenOSEntrypointData->VmmStart;
  Kmm.virtual_memory_map_top = IsenOSEntrypointData->VmmEnd;

  printf ("VirtualMemory: %x - %x (%d bytes) \n", IsenOSEntrypointData->VmmStart, IsenOSEntrypointData->VmmEnd,
		  IsenOSEntrypointData->VmmEnd - IsenOSEntrypointData->VmmStart);
  printf ("Kmm.pmm_ram_range_size %d \n", Kmm.prm_ram_range_size);

  {
	printf ("Adding kernel page to Physical Ram Manager\n");
	for (int index = 0; index < 32; index++)
	  {
		KERNEL_MEMORY_MAPPING kernel_memory_mapping = IsenOSEntrypointData->KernelMemoryMappings[index];
		if (kernel_memory_mapping.PAGES > 0)
		  {
			printf ("> %d PHYSICAL_BASE = %016"PRIx64" PHYSICAL_TOP = %016"PRIx64"\n", index, kernel_memory_mapping.PhysicalBase,
					kernel_memory_mapping.PhysicalBase
					+ kernel_memory_mapping.PAGES * ISENOS_PAGE_SIZE);

			struct PRM_RAM_RANGE *kernel_ram_range = PRMAddRange (kernel_memory_mapping.PhysicalBase,
																  kernel_memory_mapping.PhysicalBase
																  + kernel_memory_mapping.PAGES * ISENOS_PAGE_SIZE);
			printf (">> INDEX = %d, PHYSICAL_BASE = %016"PRIx64" PHYSICAL_TOP = %016"PRIx64"\n", kernel_ram_range->index, kernel_ram_range->start, kernel_ram_range->end);

		  }

	  }
  }

  PAMInit ();

  // Identity map the old VMT
  // Remove them once we updated them
  {
	struct PAGE_ALLOCATION_MANAGER_ALLOCATION *pam_allocation = PAMGetAllocationForPhysicalAddress (Kmm.virtual_memory_map_base);
	int pages = 0;
	int required_pages = ceil ((float)(((double)Kmm.virtual_memory_map_top - (double)Kmm.virtual_memory_map_base)
									   / (double)ISENOS_PAGE_SIZE));
	printf ("Memory allocation for bootloader-defined pages (OLD VMT) requires %d pages\n", required_pages);


	// We identity-map the old pages
	for (int index = 0; index < required_pages; index++)
	  {

		PAMAddAllocation (
			Kmm.virtual_memory_map_base + (ISENOS_PAGE_SIZE * index),
			Kmm.virtual_memory_map_base + (ISENOS_PAGE_SIZE * index),
			PAMA_FLAG_VMT | PAMA_FLAG_KERNEL | PAMA_FLAG_PRESENT);
	  }
  }

  // Map the kernel
  {
	printf ("Memory allocation for bootloader-defined pages (KERNEL)\n");
	// We map the kernel pages
	for (int index = 0; index < 32; index++)
	  {
		KERNEL_MEMORY_MAPPING kernel_memory_mapping = IsenOSEntrypointData->KernelMemoryMappings[index];
		for (int page = 0; page < kernel_memory_mapping.PAGES; page++)
		  {
			PAMAddAllocation (
				kernel_memory_mapping.PhysicalBase + (ISENOS_PAGE_SIZE * page),
				kernel_memory_mapping.VirtualBase + (ISENOS_PAGE_SIZE * page), PAMA_FLAG_PRESENT | PAMA_FLAG_KERNEL);
		  }

	  }
  }
// Map the kernel
  {
	struct PAGE_ALLOCATION_MANAGER_ALLOCATION *pam_allocation = PAMGetAllocationForPhysicalAddress (Kmm.pam_base);
	int pages = 0;
	int required_pages = ceil ((float)(((double)Kmm.pam_top - (double)Kmm.pam_base)
									   / (double)ISENOS_PAGE_SIZE));
	printf ("Memory allocation for PAM pages requires %d pages\n", required_pages);

	// We map the kernel pages
	for (int index = 0; index < required_pages; index++)
	  {
		PAMAddAllocation (
			Kmm.pam_base + (ISENOS_PAGE_SIZE * index),
			PAM_BASE + (ISENOS_PAGE_SIZE * index), PAMA_FLAG_PRESENT | PAMA_FLAG_KERNEL);
	  }
  }

  // Init the static memory mappings
  VMTInitStaticMappings ();

  // Print allocated physical memory pages
  PAMDebugPrint ();

  // Generate VirtualMemory tables (to be stored in CR3)
  VMTMUpdate (0);

  bkpt ();

  return 0;
}