//
// Created by rfleuryleveso on 20/04/23.
//

#include "apic.h"
#include "../print.h"
#include "inttypes.h"

uint8_t check_apic ()
{
  uint32_t eax = 0, ebx = 0, ecx = 0, edx = 0;
  cpuid (1, &eax, &ebx, &ecx, &edx);
  printf ("EAX = %d, EBX = %d, ECX = %d, EDX = %d", eax, ebx, ecx, edx);
  return edx & CPUID_FEAT_EDX_APIC;
}

static inline uint64_t rdmsr (uint32_t msr)
{
  uint32_t low, high;
  asm volatile ("rdmsr" : "=a" (low), "=d" (high) : "c" (msr));
  return ((uint64_t)high << 32) | low;
}

static inline void wrmsr (uint32_t msr, uint64_t value)
{
  uint32_t low = (uint32_t)value;
  uint32_t high = (uint32_t)(value >> 32);
  asm volatile ("wrmsr"::"a" (low), "d" (high), "c" (msr));
}

uint64_t cpu_get_apic_base ()
{
  return rdmsr (IA32_APIC_BASE_MSR);
}

uint64_t cpu_get_apic_base_address ()
{
  return rdmsr (IA32_APIC_BASE_MSR) & 0xfffff000;
}

void enable_apic ()
{
  printf ("CHECK_APIC= %016"PRIx64 "\n", check_apic ());
  // Enable the LAPIC and set the LAPIC base address
  uint64_t apic_base = cpu_get_apic_base_address ();
  printf ("APIC_BASE= %016"PRIx64 "\n", apic_base);
  apic_base |= IA32_APIC_BASE_MSR_ENABLE | IA32_APIC_BASE_MSR_BSP; // Enable LAPIC
  printf ("APIC_BASE (after enable) = %016"PRIx64 "\n", apic_base);
  wrmsr (IA32_APIC_BASE_MSR, apic_base);
  uint64_t apic_msr_value = cpu_get_apic_base ();
  printf ("APIC_BASE (verify) = %016"PRIx64 "\n", apic_base);

  uint64_t apic_base_address = cpu_get_apic_base_address ();
  uint32_t *lapic = (void *)apic_base_address;
  printf ("APIC_BASE_ADDRESS = %016"PRIx64 "\n", cpu_get_apic_base_address ());
  printf ("LAPIC_ID = %016"PRIx64 " @ %016"PRIx64"\n", *((uint8_t *)apic_base_address + 0x20), (apic_base_address
																								+ 0x20));
  printf ("LAPIC_VERSION = %016"PRIx64 " @ %016"PRIx64"\n", *((uint8_t *)apic_base_address + 0x30), (apic_base_address
																									 + 0x30));

}

void disable_apic ()
{
  printf ("CHECK_APIC= %016"PRIx64 "\n", check_apic ());
  // Enable the LAPIC and set the LAPIC base address
  uint64_t apic_base = cpu_get_apic_base_address ();
  printf ("APIC_BASE= %016"PRIx64 "\n", apic_base);
  apic_base &= ~(IA32_APIC_BASE_MSR_ENABLE);
  // apic_base |= IA32_APIC_BASE_MSR_ENABLE | IA32_APIC_BASE_MSR_BSP; // Enable LAPIC
  printf ("APIC_BASE (after disable) = %016"PRIx64 "\n", apic_base);
  wrmsr (IA32_APIC_BASE_MSR, apic_base);
  uint64_t apic_msr_value = cpu_get_apic_base ();
  printf ("APIC_BASE (verify) = %016"PRIx64 "\n", apic_base);

}