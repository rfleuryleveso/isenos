//
// Created by rfleuryleveso on 18/04/23.
//

#ifndef _VIRTUAL_MEMORY_TABLES_MANAGER_H_
#define _VIRTUAL_MEMORY_TABLES_MANAGER_H_

#include "stdint.h"

/**
 * This will regenerate the correct tables
 * @return PML4 base, for CR3
 */
uint64_t VMTMGenerateTables();

#endif //_VIRTUAL_MEMORY_TABLES_MANAGER_H_
