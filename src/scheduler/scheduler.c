//
// Created by rfleuryleveso on 21/04/23.
//

#include "scheduler.h"
#include "../interrupts/interrupts.h"
#include "../print.h"
#include "../pit/pit.h"

scheduler_t kernel_scheduler = {0};

void scheduler_tick ()
{
  disable_interrupts ();

  enable_interrupts ();
}

scheduler_program_descriptor_t *find_free_scheduler ()
{
  for (int i = 0; i < PROGRAM_DESCRIPTORS_COUNT; i++)
	{
	  if ((kernel_scheduler.program_descriptors[i].status & PROGRAM_STATUS_ALLOCATED) == 0)
		{
		  return &kernel_scheduler.program_descriptors[i];
		}
	}
}

scheduler_program_descriptor_t *scheduler_start_program (isenos_programs_e program)
{
  // Setup the heap
  scheduler_program_descriptor_t *program_descriptor = find_free_scheduler ();
  if (!program_descriptor)
	{
	  printf ("Could not find a free program descriptor\n");
	  return 0;
	}
  program_descriptor->program = program;
  program_descriptor->status |= PROGRAM_STATUS_ALLOCATED | PROGRAM_STATUS_RUNNING;
  program_descriptor->program_id = pit_counter_1000hz;

  kernel_scheduler.current_program = program_descriptor;

  return program_descriptor;
}
void scheduler_handle_scancode (uint64_t scancode)
{
  if (scancode == 0x1E)
	{ // L_CTRL pressed
	  kernel_scheduler.keyboard_flags |= SCHEDULER_KBD_FLAGS_CTRL_PRESSED;
	}
  else if (scancode == 0x9E)
	{// L_CTRL released

	  kernel_scheduler.keyboard_flags &= ~SCHEDULER_KBD_FLAGS_CTRL_PRESSED;
	}
  else if (scancode == 0x2E)
	{// C released
	  kernel_scheduler.keyboard_flags |= SCHEDULER_KBD_FLAGS_C_PRESSED;
	}
  else if (scancode == 0xAE)
	{// L_CTRL released
	  kernel_scheduler.keyboard_flags &= ~SCHEDULER_KBD_FLAGS_C_PRESSED;
	}

  if ((kernel_scheduler.keyboard_flags & SCHEDULER_KBD_FLAGS_CTRL_PRESSED) == SCHEDULER_KBD_FLAGS_CTRL_PRESSED
	  && (kernel_scheduler.keyboard_flags & SCHEDULER_KBD_FLAGS_C_PRESSED) == SCHEDULER_KBD_FLAGS_C_PRESSED)
	{
	  if (kernel_scheduler.current_program != 0
		  && (kernel_scheduler.current_program->status & PROGRAM_STATUS_RUNNING) == PROGRAM_STATUS_RUNNING)
		{
		  kernel_scheduler.current_program->status |= PROGRAM_STATUS_STOPPING;
		}
	}
}
uint8_t scheduler_program_should_stop ()
{
  return (kernel_scheduler.current_program != 0
		  && (kernel_scheduler.current_program->status & PROGRAM_STATUS_STOPPING) == PROGRAM_STATUS_STOPPING);
}
