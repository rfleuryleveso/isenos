import math
f = open("out/interrupt_wrapper.asm", "w")

f.write('''BITS 64
extern interrupt_handler\n''')
for x in range(256):
    f.write('''global      isr_handler_%d\n''' % (x))
for x in range(256):
    f.write('''extern      interrupt_handler_%d\n''' % (x))

for x in range(256):
    f.write('''isr_handler_%d:
    cli
    push rax      ;save current rax
    push rbx      ;save current rbx
    push rcx      ;save current rcx
    push rdx      ;save current rdx
    push rbp      ;save current rbp
    push rdi      ;save current rdi
    push rsi      ;save current rsi
    push r8         ;save current r8
    push r9         ;save current r9
    push r10      ;save current r10
    push r11      ;save current r11
    push r12      ;save current r12
    push r13      ;save current r13
    push r14      ;save current r14
    push r15      ;save current r15
    pushfq
    cld    ; C code following the sysV ABI requires DF to be clear on function idt_entries
    call interrupt_handler_%d
    popfq
    pop r15         ;restore current r15
    pop r14         ;restore current r14
    pop r13         ;restore current r13
    pop r12         ;restore current r12
    pop r11         ;restore current r11
    pop r10         ;restore current r10
    pop r9         ;restore current r9
    pop r8         ;restore current r8
    pop rsi         ;restore current rsi
    pop rdi         ;restore current rdi
    pop rbp         ;restore current rbp
    pop rdx         ;restore current rdx
    pop rcx         ;restore current rcx
    pop rbx         ;restore current rbx
    pop rax         ;restore current rax
    sti
    iretq
    \n''' % (x, x))

f = open("out/idt.h", "w")

f.write('''//
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
''')
for x in range(256):
    f.write('''extern void isr_handler_%d(void);\n''' % (x))

for x in range(256):
    f.write(''' void __attribute__((optimize("O0"))) interrupt_handler_%d ();\n''' % (x))

f.write('''
__attribute__((aligned(0x10)))
extern idt_entry_t idt_entries[256];
extern volatile uint64_t last_called_isr;

void init_idt();

#endif //_IDT_H_''')


f = open("out/idt.c", "w")

f.write('''//
// Created by rfleuryleveso on 20/04/23.
//

#include "idt.h"
#include "../print.h"
#include "inttypes.h"

__attribute__((aligned(0x10)))
idt_entry_t idt_entries[256];
volatile uint64_t last_called_isr = 0;

idt_ptr_t idt_ptr;

#pragma GCC push_options
#pragma GCC optimize ("O0")
// Interrupt handler routine for IRQ0

struct regs {
	uint64_t eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
};


#pragma GCC pop_options

// Print information about an IDT entry
void print_idt_entry (idt_entry_t *entry)
{
  // printf("IDT entry %d:\\n", i);
  printf ("Offset: %016lx\\n", ((uint64_t)entry->offset_high << 32) | (entry->offset_mid << 16) | entry->offset_low);
  printf ("Segment selector: %04x\\n", entry->selector);
  printf ("IST: %d\\n", entry->ist);
  printf ("Flags: %02x\\n", entry->type_attr);
}

void register_idt_entry(uint8_t index, uint8_t type_attr, uint64_t handler) {
  idt_entries[index].offset_low = (uint16_t)(handler & 0xFFFF);
  idt_entries[index].selector = 0x08; // code segment selector
  idt_entries[index].ist = 0;
  idt_entries[index].type_attr = type_attr; // interrupt gate type and attributes for 64-bit interrupts
  idt_entries[index].offset_mid = (uint16_t)((handler >> 16) & 0xFFFF);
  idt_entries[index].offset_high = (uint32_t)((handler >> 32) & 0xFFFFFFFF);
  idt_entries[index].zero = 0;
}
// Function to initialize the IDT
void init_idt ()
{
  last_called_isr = 0;
  memset (&idt_entries[0], sizeof (idt_entries), 0);
''')

for x in range(256):
    f.write('''   register_idt_entry(%d, 0x8E, (uint64_t )&isr_handler_%d);\n''' % (x, x))
f.write('''// Load the IDT
  idt_ptr.base = (uint64_t)&idt_entries;
  // idt_ptr.size = sizeof(idt_entries) - 1;
  idt_ptr.size = 4095;

  printf ("IDT_PTR = %016"PRIx64 " IDT =%016"PRIx64 "\\n", &idt_ptr, &idt_entries[0]);
  // Enable interrupts
  uint64_t ptr = (uint64_t)&idt_ptr; // get the pointer to your GDT table as a 32bit int
  asm volatile("movq %0,%%rax    \\n\\t"  //load the pointer int eax
			   "lidt (%%rax) "            //Set the CPU GDT pointer to your table
	  : : "r" (ptr));

  printf ("INTERRUPT TABLE UPDATED !! \\n");

}''')

for x in range(256):
    f.write(''' void __attribute__((optimize("O0"))) interrupt_handler_%d ()
{
  printf (">>>>>>>> INTERRUPT %d ! \\n");
  outb (0x20, %d);
  outb (0xa0, %d);
}\n''' % (x, x, x, x))


f = open("out/gdb_commands.txt", "w")
for x in range(256):
    f.write('''break interrupt_handler_%d\n''' % (x))
f.write('''break main
continue''')