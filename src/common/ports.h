//
// Created by rfleuryleveso on 17/04/23.
//

#ifndef ISENOS_PRELOADER_MACROS_H
#define ISENOS_PRELOADER_MACROS_H

#include "stdint.h"

uint8_t inb (uint16_t _port);
void outb (uint16_t _port, uint8_t _data);
uint16_t inw (uint16_t _port);
void outw (uint16_t _port, uint16_t _data);

#define bkpt() __asm__("int3")

#define ALIGN_PTR(ADDRESS, ALIGN)   ((uintptr_t)ADDRESS & -ALIGN)

#define ALIGN_PTR_UPWARD(ptr, alignment) \
    (((uintptr_t)(ptr) + (alignment - 1)) & ~(uintptr_t)(alignment - 1))

#define ALIGN_PTR_DOWNWARD(ptr, alignment) \
    ((uintptr_t)(ptr) & ~(uintptr_t)(alignment - 1))

#endif //ISENOS_PRELOADER_MACROS_H
