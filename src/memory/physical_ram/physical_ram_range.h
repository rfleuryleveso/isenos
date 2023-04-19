//
// Created by rfleuryleveso on 18/04/23.
//

#ifndef ISENOS_PRELOADER_PHYSICAL_RAM_RANGE_H
#define ISENOS_PRELOADER_PHYSICAL_RAM_RANGE_H

#include "stdint.h"

#define PRMRR_FLAG_KERNEL 0x2
#define PRMRR_FLAG_USED 0x1

struct PRM_RAM_RANGE {
    uint8_t flags;
    uint16_t index;
    uint64_t start;
    uint64_t end;
};


#endif //ISENOS_PRELOADER_PHYSICAL_RAM_RANGE_H
