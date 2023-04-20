//
// Created by rfleuryleveso on 18/04/23.
//
#include "static_mappings.h"

struct VMT_STATIC_MAPPING static_mappings[VMT_STATIC_MAPPING_COUNT] = { 0 };

void vmt_init_static_mappings ()
{
  // Find the highest memory address
  // This is required because the Physical Ram Manager ranges is not sorted
  uint64_t highest_memory_address;
  for (uint64_t index = 0; index < Kmm.prm_ram_range_size; index++)
	{
	  if (Kmm.prm_ram_ranges[Kmm.prm_ram_range_size - 1].end > highest_memory_address)
		{
		  highest_memory_address = Kmm.prm_ram_ranges[Kmm.prm_ram_range_size - 1].end;
		}
	}

  static_mappings[0].virtual_address = PHM_BASE;
  static_mappings[0].physical_address = 0;
  static_mappings[0].pages = ceil (highest_memory_address / ISENOS_PAGE_SIZE);
  static_mappings[0].enabled = 1;

  static_mappings[1].virtual_address = ACPI_BASE;
  static_mappings[1].physical_address = Kmm.acpi_base;
  static_mappings[1].pages = 1;
  static_mappings[1].enabled = 1;

  printf ("vmt_init_static_mappings real memory mapped into kernel with %016"PRIx64" pages \n", static_mappings[0].pages);
}