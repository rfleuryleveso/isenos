//
// Created by rfleuryleveso on 19/04/23.
//

#ifndef _MEMORY_TOOLS_H_
#define _MEMORY_TOOLS_H_

#include "stdint.h"

uint64_t MemPhysToVirt(uint64_t physical_address);
uint64_t MemVirtToPhys(uint64_t virtual_address);

#endif //_MEMORY_TOOLS_H_
