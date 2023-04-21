//
// Created by rfleuryleveso on 21/04/23.
//

#include "pit.h"
#include "../common/ports.h"

volatile uint64_t pit_counter_1000hz = 0;

void init_pit(uint32_t frequency) {
  uint32_t divisor = 1193180 / frequency;

  // Set the PIT to mode 0 (interrupt on terminal count)
  // and set the PIT to operate in binary mode
  outb(PIT_COMMAND_PORT, PIT_CHANNEL0 | PIT_MODE0 | PIT_BINARY);

  // Write the lower byte of the divisor to channel 0
  outb(PIT_CHANNEL0_DATA_PORT, divisor & 0xFF);

  // Write the upper byte of the divisor to channel 0
  outb(PIT_CHANNEL0_DATA_PORT, (divisor >> 8) & 0xFF);
}

void wait_ms(uint64_t count) {
  uint64_t target_pit = pit_counter_1000hz + count;
  while(pit_counter_1000hz < target_pit) {
	asm("hlt");
  }
}