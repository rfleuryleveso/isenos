//
// Created by rfleuryleveso on 21/04/23.
//

#include "sysinfo.h"
#include "../print.h"
#include "../memory/kernel_memory_information.h"
#include "../memory/page_allocation/page_allocation_manager.h"
#include "../pit/pit.h"
#include "../graphics/graphics_manager.h"
void sys_info ()
{
  printf_ui ("\nSYSTEM INFO\n");
  printf_ui ("KMM_BASE 	: %016"PRIx64"\n", &Kmm);
  printf_ui ("MEM_AVAIL 	: %016"PRIx64"\n", Kmm.available_memory);

  page_allocation_manager_allocation_t *base_allocation = pam_get_base ();

  printf_ui ("PAM_ALLOCS	: 0x%x\n", Kmm.pam_allocations_count);
  // For each pam idt_entries
  for (int pam_allocation_index = 0; pam_allocation_index < Kmm.pam_allocations_count; pam_allocation_index++)
	{
	  page_allocation_manager_allocation_t *allocation = &base_allocation[pam_allocation_index];
	  if ((allocation->flags & PAMA_FLAG_PRESENT) == PAMA_FLAG_PRESENT)
		{
		  printf_ui ("PAM_ALLOC[%d]	: 				\n", pam_allocation_index);
		  printf_ui ("> P				:  %016"PRIx64"\n", allocation->physical_start);
		  printf_ui ("> V				:  %016"PRIx64"\n", allocation->virtual_start);
		}
	  wait_ms(500);
	  gm_clear();
	  gm_render();
	}

}
