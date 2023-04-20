//
// Created by rfleuryleveso on 20/04/23.
//

#ifndef _APIC_H_
#define _APIC_H_
#include "stdint.h"
#include "../common/msr.h"

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE 0x800


/* Set the physical address for local APIC registers */
void cpu_set_apic_base(uintptr_t apic) ;

/**
 * Get the physical address of the APIC registers page
 * make sure you map it to virtual memory ;)
 */
uint64_t cpu_get_apic_base() ;

void enable_apic();
void disable_apic();

#endif //_APIC_H_
