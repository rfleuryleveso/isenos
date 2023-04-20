//
// Created by rfleuryleveso on 20/04/23.
//

#include "interrupts.h"
#include "../idt/idt.h"
#include "apic.h"
#include "pic.h"
#include "../common/ports.h"

void setup_interrupts ()
{

  // Setup IDT
  init_idt ();

  // Disable the old interrupt controller
  interrupts_pic_init ();

  // Enable the APIC
  // enable_apic ();
  disable_apic ();


}
void disable_interrupts (void)
{
  // Disable interrupts
  __asm__ volatile ("cli");
}

void enable_interrupts (void)
{
  // Enable interrupts
  __asm__ volatile ("sti");
}