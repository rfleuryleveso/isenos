//
// Created by rfleuryleveso on 21/04/23.
//

#include "scheduler.h"
#include "../interrupts/interrupts.h"
void scheduler_tick ()
{
  disable_interrupts ();

  enable_interrupts ();
}
scheduler_program_descriptor_t *scheduler_start_program (isenos_programs_e program)
{
  // Setup the heap
}
