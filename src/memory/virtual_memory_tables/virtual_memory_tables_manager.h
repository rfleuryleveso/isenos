//
// Created by rfleuryleveso on 18/04/23.
//

#ifndef _VIRTUAL_MEMORY_TABLES_MANAGER_H_
#define _VIRTUAL_MEMORY_TABLES_MANAGER_H_

#include "../kernel_memory_information.h"
#include "../../print.h"
#include "stdint.h"
#include "inttypes.h"
#include "vm.h"
#include "../page_allocation/page_allocation_manager.h"
#include "../../common/memory.h"
#include "../../common/math.h"


/**
 * Update the virtual memory map
 * @param identity_mapped_memory
 * @return
 */
uint64_t vmtm_update (uint8_t identity_mapped_memory);

#endif //_VIRTUAL_MEMORY_TABLES_MANAGER_H_
