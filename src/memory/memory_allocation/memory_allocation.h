//
// Created by rfleuryleveso on 20/04/23.
//

#ifndef _MEMORY_ALLOCATION_H_
#define _MEMORY_ALLOCATION_H_

#include "stdint.h"

typedef struct memory_allocation_t {
  uint64_t physical_address;
  uint64_t virtual_address;
  uint64_t size;

  uint64_t references_count;
  uint64_t program_id;

  uint64_t flags;
};

#endif //_MEMORY_ALLOCATION_H_
