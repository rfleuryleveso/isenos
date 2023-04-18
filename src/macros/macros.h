//
// Created by rfleuryleveso on 17/04/23.
//

#ifndef ISENOS_PRELOADER_MACROS_H
#define ISENOS_PRELOADER_MACROS_H

#include "stdint.h"

uint8_t inb (uint16_t _port);
void outb (uint16_t _port, uint8_t _data);

void bkpt();

#endif //ISENOS_PRELOADER_MACROS_H
