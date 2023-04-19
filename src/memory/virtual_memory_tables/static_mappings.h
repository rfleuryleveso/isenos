//
// Created by rfleuryleveso on 18/04/23.
//

#ifndef _STATIC_MAPPINGS_H_
#define _STATIC_MAPPINGS_H_

#include "stdint.h"
#include "../../common/os-config.h"
#include "../kernel_memory_information.h"
#include "../../common/math.h"
#include "../../print.h"
#include "stdint.h"
#include "inttypes.h"

#define VMT_STATIC_MAPPING_COUNT 1

#define VMT_STATIC_MAPPING_FLAGS_PHYS 0x1

struct VMT_STATIC_MAPPING {
	uint8_t enabled;
	uint64_t virtual_address;
	uint64_t physical_address;
	uint64_t pages;
	uint64_t flags;
};

extern struct VMT_STATIC_MAPPING static_mappings[VMT_STATIC_MAPPING_COUNT];

void vmt_init_static_mappings ();

#endif //_STATIC_MAPPINGS_H_
