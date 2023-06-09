//
// Created by rfleuryleveso on 19/04/23.
//

#include "gdt.h"
#include "../print.h"
#include "inttypes.h"
#include "../memory/memory_tools.h"

uint64_t gdt_table[5] = {0};
struct gdt_ptr gdt_table_ptr;

void
create_descriptor(uint16_t index, uint32_t base, uint32_t limit, uint16_t flag)
{
  uint64_t descriptor;

  // Create the high 32 bit segment
  descriptor  =  limit       & 0x000F0000;         // set size bits 19:16
  descriptor |= (flag <<  8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
  descriptor |= (base >> 16) & 0x000000FF;         // set base bits 23:16
  descriptor |=  base        & 0xFF000000;         // set base bits 31:24

  // Shift by 32 to allow for low part of segment
  descriptor <<= 32;

  // Create the low 32 bit segment
  descriptor |= base  << 16;                       // set base bits 15:0
  descriptor |= limit  & 0x0000FFFF;               // set size bits 15:0

  gdt_table[index] = descriptor;
}

void setup_gdt ()
{
  memset(&gdt_table[0], 0, sizeof(gdt_table));

  create_descriptor(0, 0, 0, 0);
  create_descriptor(1, 0, 0x000FFFFF, (GDT_CODE_PL0));
  create_descriptor(2, 0, 0x000FFFFF, (GDT_DATA_PL0));
  create_descriptor(3, 0, 0x000FFFFF, (GDT_CODE_PL3));
  create_descriptor(4, 0, 0x000FFFFF, (GDT_DATA_PL3));
//  create_descriptor(3, 0, 0x000FFFFF, (GDT_CODE_PL3));
//  create_descriptor(4, 0, 0x000FFFFF, (GDT_DATA_PL3));
  printf ("[GDT] Writing GDT table @ %016"PRIx64" with LGDT \n", &gdt_table_ptr);
  gdt_table_ptr.base = (uint64_t )&gdt_table[0];
  gdt_table_ptr.limit = sizeof(gdt_table)-1;

  uint64_t ptr = (uint64_t)&gdt_table_ptr; // get the pointer to your GDT table as a 32bit int

  asm ("cli\n");
  asm volatile("movq %0,%%rax    \n\t"  //load the pointer int eax
			   "lgdt (%%rax) "            //Set the CPU GDT pointer to your table
			   : : "r" (ptr));

  flush_gdt();
  asm ("sti\n");
}
