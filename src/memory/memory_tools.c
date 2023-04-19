//
// Created by rfleuryleveso on 19/04/23.
//

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
  char *pszDest = (char *)dst;
  const char *pszSource = (const char *)src;
  if ((pszDest != NULL) && (pszSource != NULL))
	{
	  while (size) //till cnt
		{
		  //Copy byte by byte
		  *(pszDest++) = *(pszSource++);
		  --size;
		}
	}
  return dst;
}
