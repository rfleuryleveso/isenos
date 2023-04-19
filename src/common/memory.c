//
// Created by rfleuryleveso on 18/04/23.
//

#include "memory.h"
void MemoryZero (void *address, uint64_t size)
{
  for (uint8_t *current_address = address; current_address < (uint8_t *)address + size; current_address++)
	{
	  *current_address = 0;
	}
}
