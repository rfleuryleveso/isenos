#include "inttypes.h"
#include "stdint.h"
#include "isenos.h"
#include "print.h"
#include "memory/efi_memory_types.h"
#include "memory/kernel_memory_information.h"
#include "memory/physical_ram/physical_ram_manager.h"
#include "memory/ram_allocation/ram_allocation_manager.h"
#include "common/os-config.h"
#include "memory/ram_allocation/ram_allocation_manager_allocation.h"

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
  printf ("Kmm.pmm_ram_range_size %d \n", Kmm.pmm_ram_range_size);

  RAMInit();

  bkpt();

  return 0;
}