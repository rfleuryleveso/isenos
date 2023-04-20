//
// Created by rfleuryleveso on 20/04/23.
//

#ifndef _IDT_H_
#define _IDT_H_
#include <stdint.h>
#include "../memory/memory_tools.h"

// Interrupt table structure
typedef struct {
	uint16_t offset_low;  // 16 bits of the interrupt handler address
	uint16_t selector;    // code segment selector
	uint8_t  ist;         // interrupt stack table index
	uint8_t  type_attr;   // interrupt gate type and attributes
	uint16_t offset_mid;  // 16 middle bits of the interrupt handler address
	uint32_t offset_high; // 32 higher bits of the interrupt handler address
	uint32_t zero;        // always set to 0
} __attribute__((packed)) __attribute__((aligned(0x10)))  idt_entry_t;

// Interrupt table pointer structure
typedef struct {
	uint16_t size;      // table size -1
	uint64_t base;       // table address
} __attribute__((packed)) __attribute__((aligned(0x10)))  idt_ptr_t;

extern void isr_wrapper(void);
extern void isr_wrapper_kbd(void);

__attribute__((aligned(0x10)))
extern idt_entry_t idt_entries[256];

void init_idt();

#endif //_IDT_H_
