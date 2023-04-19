//
// Created by rfleuryleveso on 17/04/23.
//

#ifndef ISENOS_PRELOADER_PHYSICAL_RAM_MANAGER_H
#define ISENOS_PRELOADER_PHYSICAL_RAM_MANAGER_H

#include "stdint.h"
#include "physical_ram_range.h"
#include "../kernel_memory_information.h"

struct PRM_RAM_RANGE *prm_add_range (uint64_t start, uint64_t end);

#endif //ISENOS_PRELOADER_PHYSICAL_RAM_MANAGER_H
