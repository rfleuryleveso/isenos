//
// Created by rfleuryleveso on 17/04/23.
//

#include "physical_ram_manager.h"

struct PRM_RAM_RANGE * prm_add_range(uint64_t start, uint64_t end) {
    // Retrieve a pointer to the current RAM range
    struct PRM_RAM_RANGE* CurrentPmmRamRange = &Kmm.prm_ram_ranges[Kmm.prm_ram_range_size];

    CurrentPmmRamRange->start = start;
    CurrentPmmRamRange->end = end;
    CurrentPmmRamRange->flags |= PRMRR_FLAG_USED;

    // We must increase the ram range size
    CurrentPmmRamRange->index |= Kmm.prm_ram_range_size++;

	return CurrentPmmRamRange;
}