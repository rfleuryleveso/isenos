//
// Created by rfleuryleveso on 20/04/23.
//

#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_
#include <stdint.h>

void disable_8259_pic();

void setup_interrupts();

void disable_interrupts();

void enable_interrupts();

#endif //_INTERRUPTS_H_
