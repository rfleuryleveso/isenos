//
// Created by rfleuryleveso on 18/04/23.
//
#include "static_mappings.h"

struct VMT_STATIC_MAPPING static_mappings[VMT_STATIC_MAPPING_COUNT] = { 0 };

void VMTInitStaticMappings ()
{
  static_mappings[0].virtual_address = PHM_BASE;
  static_mappings[0].physical_address = 0;
  static_mappings[0].pages = ceil (Kmm.prm_ram_ranges[Kmm.prm_ram_range_size - 1].end / ISENOS_PAGE_SIZE);
  static_mappings[0].enabled = 1;


  printf ("VMTInitStaticMappings real memory mapped into kernel with %016 pages"PRIx64" \n", static_mappings[0].pages);
}