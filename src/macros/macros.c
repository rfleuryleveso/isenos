//
// Created by rfleuryleveso on 17/04/23.
//

#include "macros.h"

uint8_t inb (uint16_t _port)
{
    uint8_t rv;
    __asm__ __volatile__ ("inb %w1,%b0" : "=a" (rv) : "d" (_port));
    return rv;
}

void outb (uint16_t _port, uint8_t _data)
{
    __asm__ __volatile__ ("outb %b0,%w1" : : "a" (_data), "d" (_port));
}

void bkpt ()
{
    __asm__("int3");
}

