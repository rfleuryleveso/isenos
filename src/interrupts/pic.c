//
// Created by rfleuryleveso on 20/04/23.
//

#include "pic.h"
#include "../common/ports.h"
#include "../idt/idt.h"
void pic_sendendofeoi(unsigned char irq)
{
  if(irq >= 8)
	{
	  outb (PIC2_CMD, PIC_EOI);
	}
  outb(PIC1_CMD,PIC_EOI);
}

void interrupts_pic_init ()
{
  // ICW1: start initialization, ICW4 needed
  outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
  outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);

  // ICW2: interrupt vector address
  outb(PIC1_DATA, 0x20);
  outb(PIC2_DATA, 0x28);

  // ICW3: master/slave wiring
  outb(PIC1_DATA, 4);
  outb(PIC2_DATA, 2);

  outb(PIC1_DATA, ICW4_8086);
  outb(PIC2_DATA, ICW4_8086);

  // OCW1: Disable all IRQs
  outb(PIC1_DATA, ~0x2);  // 11111100
  outb(PIC2_DATA, ~0x0);  // 11111111

}
