//
// Created by rfleuryleveso on 19/04/23.
//

#include "../common/os-types.h"
#include "memory_tools.h"
#include "../common/os-config.h"

uint64_t mem_phys_to_virt (uint64_t physical_address)
{
  return physical_address + PHM_BASE;
}
uint64_t mem_virt_to_phys (uint64_t virtual_address)
{
  return virtual_address - PHM_BASE;
}

void *memcpy (void *dst, void *src, uint64_t size)
{
  char *psz_dest = (char *)dst;
  const char *psz_source = (const char *)src;
  if ((psz_dest != NULL) && (psz_source != NULL))
	{
	  while (size) //till cnt
		{
		  //Copy byte by byte
		  *(psz_dest++) = *(psz_source++);
		  --size;
		}
	}
  return dst;
}
void memset (void *address, uint64_t size, uint64_t value)
{
  for (uint8_t *current_address = address; current_address < (uint8_t *)address + size; current_address++)
	{
	  *current_address = value;
	}
}
