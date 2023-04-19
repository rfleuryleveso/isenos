//
// Created by rfleuryleveso on 19/04/23.
//

#ifndef _MEMORY_TOOLS_H_
#define _MEMORY_TOOLS_H_

#include "stdint.h"

uint64_t mem_phys_to_virt (uint64_t physical_address);
uint64_t mem_virt_to_phys (uint64_t virtual_address);
void *memcpy (void *dst, void *src, uint64_t size);

#endif //_MEMORY_TOOLS_H_
