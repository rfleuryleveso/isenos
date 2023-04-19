//
// Created by rfleuryleveso on 19/04/23.
//

#include "memory_tools.h"
#include "../common/os-config.h"
uint64_t MemPhysToVirt (uint64_t physical_address)
{
  return physical_address + PHM_BASE;
}
uint64_t MemVirtToPhys (uint64_t virtual_address)
{
  return virtual_address - PHM_BASE;
}
