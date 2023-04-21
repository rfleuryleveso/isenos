//
// Created by rfleuryleveso on 21/04/23.
//

#ifndef _PIT_H_
#define _PIT_H_

#include "inttypes.h"
#define PIT_CHANNEL0_DATA_PORT 0x40
#define PIT_CHANNEL1_DATA_PORT 0x41
#define PIT_CHANNEL2_DATA_PORT 0x42
#define PIT_COMMAND_PORT       0x43

#define PIT_CHANNEL0   0
#define PIT_CHANNEL1   1
#define PIT_CHANNEL2   2

#define PIT_MODE0      0x00
#define PIT_MODE2      0x04

#define PIT_BINARY     0x00
#define PIT_BCD        0x01

// Set the PIT frequency to the desired value
void init_pit(uint32_t frequency);

/**
 * This will be incremented automatically around 1000hz.
 * Accuracy is terrible
 */
extern volatile uint64_t pit_counter_1000hz;

void wait_ms(uint64_t count);

#endif //_PIT_H_
