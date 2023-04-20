//
// Created by rfleuryleveso on 20/04/23.
//

#include "msr.h"
#include <cpuid.h>
void cpu_get_msr(uint32_t msr, uint32_t *lo, uint32_t *hi)
{
  asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}
void cpu_set_msr(uint32_t msr, uint32_t lo, uint32_t hi)
{
  asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}
void cpuid(uint32_t function, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx) {
  // Use inline assembly to execute the cpuid instruction
  __get_cpuid(1, eax, ebx, ecx, edx);
//  __asm__ volatile (
//	  "cpuid;"
//	  : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
//	  : "a" (function)
//	  );
}