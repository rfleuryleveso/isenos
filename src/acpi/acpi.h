//
// Created by rfleuryleveso on 20/04/23.
//

#ifndef _ACPI_H_
#define _ACPI_H_

#include "stdint.h"

uint64_t acpi_find_rsdp (uint64_t acpi_base_address, uint64_t acpi_top_address);

#endif //_ACPI_H_
