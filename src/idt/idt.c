//
// Created by rfleuryleveso on 20/04/23.
//

#include "idt.h"
#include "../print.h"
#include "inttypes.h"

__attribute__((aligned(0x10)))
idt_entry_t idt_entries[256];

idt_ptr_t idt_ptr;

#pragma GCC push_options
#pragma GCC optimize ("O0")
// Interrupt handler routine for IRQ0

struct regs {
	uint64_t eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
};

void __attribute__((optimize("O0"))) interrupt_handler (struct regs r)
{
  printf (">>>>>>>> INTERRUPT ! \n");
  outb (0x20, 0x20);
  outb (0xa0, 0x20);
}

void __attribute__((optimize("O0"))) interrupt_handler_kbd (struct regs r)
{
  printf (">>>>>>>> INTERRUPT KBD ! \n");
  int scancode = inb (0x60);
  printf ("Scan code: %02X\n", scancode); // print scan code in hex format

  outb (0x20, 0x20);
  outb (0xa0, 0x20);
}

#pragma GCC pop_options

// Print information about an IDT entry
void print_idt_entry (idt_entry_t *entry)
{
  // printf("IDT entry %d:\n", i);
  printf ("Offset: %016lx\n", ((uint64_t)entry->offset_high << 32) | (entry->offset_mid << 16) | entry->offset_low);
  printf ("Segment selector: %04x\n", entry->selector);
  printf ("IST: %d\n", entry->ist);
  printf ("Flags: %02x\n", entry->type_attr);
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
  memset (&idt_entries[0], sizeof (idt_entries), 0);

  for (uint32_t i = 0; i < 256; ++i)
	{
	  register_idt_entry(i, 0x8F, (uint64_t )&isr_wrapper);
	  // Interrupt gate
	}

  {
	idt_entries[21].offset_low = (uint16_t)((uint64_t)&isr_wrapper_kbd & 0xFFFF);
	idt_entries[21].selector = 0x08; // code segment selector
	idt_entries[21].ist = 0;
	idt_entries[21].type_attr = 0x8E; // interrupt gate type and attributes for 64-bit interrupts
	idt_entries[21].offset_mid = (uint16_t)(((uint64_t)&isr_wrapper_kbd >> 16) & 0xFFFF);
	idt_entries[21].offset_high = (uint32_t)(((uint64_t)&isr_wrapper_kbd >> 32) & 0xFFFFFFFF);
	idt_entries[21].zero = 0;
	// Interrupt gate
  }
  // Load the IDT
  idt_ptr.base = (uint64_t)&idt_entries;
  // idt_ptr.size = sizeof(idt_entries) - 1;
  idt_ptr.size = 4095;

  printf ("IDT_PTR = %016"PRIx64 " IDT =%016"PRIx64 "\n", &idt_ptr, &idt_entries[0]);
  // Enable interrupts
  uint64_t ptr = (uint64_t)&idt_ptr; // get the pointer to your GDT table as a 32bit int
  asm volatile("movq %0,%%rax    \n\t"  //load the pointer int eax
			   "lidt (%%rax) "            //Set the CPU GDT pointer to your table
	  : : "r" (ptr));

  printf ("INTERRUPT TABLE UPDATED !! \n");

}