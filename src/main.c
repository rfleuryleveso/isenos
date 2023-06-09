#include "inttypes.h"
#include "stdint.h"
#include "isenos.h"
#include "print.h"
#include "memory/efi_memory_types.h"
#include "memory/kernel_memory_information.h"
#include "memory/physical_ram/physical_ram_manager.h"
#include "memory/page_allocation/page_allocation_manager.h"
#include "common/os-config.h"
#include "memory/page_allocation/page_allocation_manager_allocation.h"
#include "common/math.h"
#include "memory/virtual_memory_tables/virtual_memory_tables_manager.h"
#include "memory/virtual_memory_tables/static_mappings.h"
#include "graphics/graphics_manager.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "interrupts/interrupts.h"
#include "acpi/acpi.h"
#include "disk/disk.h"
#include "graphics/text_output_manager.h"
#include "pit/pit.h"
#include "programs/colorscroll.h"
#include "console/console_manager.h"
#include "scheduler/scheduler.h"
#include "main.h"
#include "programs/sysinfo.h"
#include "programs/donut.h"
#include "programs/matrix.h"

uint64_t main (IBL_ISENOS_DATA *isen_os_entrypoint_data)
{
  // Let's assume no IDT
  disable_interrupts ();

  // Initialize Serial Port
  print_initialize_com1 ();

  // Display the Splash screen and Kmm offset
  printf (ISEN_OS_SPLASH);
  printf ("Initializing ISENOS: Kmm = %016"PRIx64"\n");
  printf ("EFI_TABLE = %016"PRIx64"\n", isen_os_entrypoint_data->EfiTablePointer);

  text_output_manager_add_string ("Loading ISENOS !\0");

  // Compute all usable ram, and add all entries to the Physical Ram Manager
  long long total_usable_ram = 0;
  for (uint32_t index = 0; index < isen_os_entrypoint_data->MemoryMapSize; index++)
	{
	  IBL_MEMORY_MAP_ENTRY map_entry = isen_os_entrypoint_data->MemoryMap[index];
	  printf ("    Entry %3d | TYPE = %x (", index, map_entry.Type);

	  struct PRM_RAM_RANGE *range;

	  switch (map_entry.Type)
		{
	  case EfiConventionalMemory:
		printf ("Free RAM            ");
		  total_usable_ram += map_entry.Size;
		  range = prm_add_range (map_entry.PhysicalStart, map_entry.PhysicalStart + map_entry.Size);
		  break;
	  case EfiACPIReclaimMemory:
		printf ("ACPI Tables         ");
		  range = prm_add_range (map_entry.PhysicalStart, map_entry.PhysicalStart + map_entry.Size);
		  range->flags |= PRMRR_FLAG_UNUSABLE | PRMRR_FLAG_ACPI;
		  Kmm.acpi_base = map_entry.PhysicalStart;
		  Kmm.acpi_top = map_entry.PhysicalStart + map_entry.Size;
		  break;
	  case EfiMemoryMappedIO:
		printf ("EfiMemoryMappedIO   ");
		  break;
		}

	  printf (") FROM = %16x TO = %16x, SIZE = %x\n", map_entry.PhysicalStart,
			  map_entry.PhysicalStart + map_entry.Size, map_entry.Size);
	}
  printf ("Total usable RAM = %"PRId64" bytes\n", total_usable_ram);
  Kmm.available_memory = total_usable_ram;
  Kmm.kernel_base = isen_os_entrypoint_data->KernelStart;
  Kmm.kernel_top = isen_os_entrypoint_data->KernelEnd;
  Kmm.virtual_memory_map_base = isen_os_entrypoint_data->VmmStart;
  Kmm.virtual_memory_map_top = isen_os_entrypoint_data->VmmEnd;

  printf ("VirtualMemory: %x - %x (%d bytes) \n", isen_os_entrypoint_data->VmmStart, isen_os_entrypoint_data->VmmEnd,
		  isen_os_entrypoint_data->VmmEnd - isen_os_entrypoint_data->VmmStart);
  printf ("Kmm.pmm_ram_range_size %d \n", Kmm.prm_ram_range_size);

  // Graphical output buffer
  gm_init (isen_os_entrypoint_data);



  // Make the kernel page visible to the PRM
  // It is needed because the Page Allocation Manager won't recognize kernel pages if they are not in the PRM
  {
	printf ("Adding kernel page to Physical Ram Manager\n");
	for (int index = 0; index < 32; index++)
	  {
		KERNEL_MEMORY_MAPPING kernel_memory_mapping = isen_os_entrypoint_data->KernelMemoryMappings[index];
		if (kernel_memory_mapping.PAGES > 0)
		  {
			printf ("> %d PHYSICAL_BASE = %016"PRIx64" PHYSICAL_TOP = %016"PRIx64"\n", index, kernel_memory_mapping.PhysicalBase,
					kernel_memory_mapping.PhysicalBase
					+ kernel_memory_mapping.PAGES * ISENOS_PAGE_SIZE);

			struct PRM_RAM_RANGE *kernel_ram_range = prm_add_range (kernel_memory_mapping.PhysicalBase,
																	kernel_memory_mapping.PhysicalBase
																	+ kernel_memory_mapping.PAGES * ISENOS_PAGE_SIZE);
			printf (">> INDEX = %d, PHYSICAL_BASE = %016"PRIx64" PHYSICAL_TOP = %016"PRIx64"\n", kernel_ram_range->index, kernel_ram_range->start, kernel_ram_range->end);

		  }

	  }
  }
  {
	printf ("Adding frame buffer to Physical Ram Manager\n");
	printf ("> %d PHYSICAL_BASE = %016"PRIx64" PHYSICAL_TOP = %016"PRIx64"\n", isen_os_entrypoint_data->FrameBufferInfo->FrameBufferBase,
			isen_os_entrypoint_data->FrameBufferInfo->FrameBufferBase
			+ isen_os_entrypoint_data->FrameBufferInfo->FrameBufferSize);
	struct PRM_RAM_RANGE *kernel_ram_range = prm_add_range (isen_os_entrypoint_data->FrameBufferInfo->FrameBufferBase,
															isen_os_entrypoint_data->FrameBufferInfo->FrameBufferBase
															+ isen_os_entrypoint_data->FrameBufferInfo->FrameBufferSize);

  }
  // Init the Page Allocation Unit
  pam_init ();

  // Setup GDT (Segment descriptors table)
  setup_gdt ();

  setup_interrupts ();

  // Identity map the old VMT
  // This is used to make sure we don't overwrite the VMT
  // Remove them once we updated them
  {
	struct page_allocation_manager_allocation_t *pam_allocation = pam_get_allocation_for_physical_address (Kmm.virtual_memory_map_base);
	int pages = 0;
	int required_pages = ceil ((float)(((double)Kmm.virtual_memory_map_top - (double)Kmm.virtual_memory_map_base)
									   / (double)ISENOS_PAGE_SIZE));
	printf ("Memory allocation for bootloader-defined pages (OLD VMT) requires %d pages\n", required_pages);


	// We identity-map the old pages
	for (int index = 0; index < required_pages; index++)
	  {

		page_allocation_manager_allocation_t *allocation = pam_add_allocation (
			Kmm.virtual_memory_map_base + (ISENOS_PAGE_SIZE * index),
			Kmm.virtual_memory_map_base + (ISENOS_PAGE_SIZE * index),
			PAMA_FLAG_VMT | PAMA_FLAG_KERNEL | PAMA_FLAG_PRESENT);

		// This should be free'd once the new CR3 VMT has been set
		allocation->references_count = 0;
	  }
  }

  // Map the kernel, making sure we don't overwrite it too.
  {
	printf ("Memory allocation for bootloader-defined pages (KERNEL)\n");
	// We map the kernel pages
	for (int index = 0; index < 32; index++)
	  {
		KERNEL_MEMORY_MAPPING kernel_memory_mapping = isen_os_entrypoint_data->KernelMemoryMappings[index];
		for (int page = 0; page < kernel_memory_mapping.PAGES; page++)
		  {
			page_allocation_manager_allocation_t *allocation = pam_add_allocation (
				kernel_memory_mapping.PhysicalBase + (ISENOS_PAGE_SIZE * page),
				kernel_memory_mapping.VirtualBase + (ISENOS_PAGE_SIZE * page), PAMA_FLAG_PRESENT | PAMA_FLAG_KERNEL);
			allocation->references_count++;
		  }

	  }
  }
  // Register the Page allocation manager's pages into the PAM
  {
	struct page_allocation_manager_allocation_t *pam_allocation = pam_get_allocation_for_physical_address (Kmm.pam_base);
	int pages = 0;
	int required_pages = ceil ((float)(((double)Kmm.pam_top - (double)Kmm.pam_base)
									   / (double)ISENOS_PAGE_SIZE));
	printf ("Memory allocation for PAM pages requires %d pages\n", required_pages);

	// We map the kernel pages
	for (int index = 0; index < required_pages; index++)
	  {
		page_allocation_manager_allocation_t *allocation = pam_add_allocation (
			Kmm.pam_base + (ISENOS_PAGE_SIZE * index),
			PAM_BASE + (ISENOS_PAGE_SIZE * index), PAMA_FLAG_PRESENT | PAMA_FLAG_KERNEL);
		allocation->references_count++;
	  }
  }
  // Page allocation for graphics
  {
	struct page_allocation_manager_allocation_t *framebuffer_allocation = pam_get_allocation_for_physical_address (isen_os_entrypoint_data->FrameBufferInfo->FrameBufferBase);
	uint64_t graphics_physical_base = isen_os_entrypoint_data->FrameBufferInfo->FrameBufferBase;
	uint64_t graphics_physical_top = isen_os_entrypoint_data->FrameBufferInfo->FrameBufferBase
									 + isen_os_entrypoint_data->FrameBufferInfo->FrameBufferSize;

	uint64_t aligment_offset = isen_os_entrypoint_data->FrameBufferInfo->FrameBufferBase
							   - ALIGN_PTR_DOWNWARD(isen_os_entrypoint_data->FrameBufferInfo->FrameBufferBase, ISENOS_PAGE_SIZE);
	int required_pages = ceil ((float)(graphics_physical_top / (double)ISENOS_PAGE_SIZE));
	printf ("Memory allocation for graphics @ %016"PRIx64 " offset = %016"PRIx64" with %d pages \n", isen_os_entrypoint_data->FrameBufferInfo->FrameBufferBase, aligment_offset, required_pages);

	for (int index = 0; index < required_pages; index++)
	  {
		page_allocation_manager_allocation_t *allocation = pam_add_allocation (
			isen_os_entrypoint_data->FrameBufferInfo->FrameBufferBase + (ISENOS_PAGE_SIZE * index),
			GRAPHICS_BUFFER_BASE + (ISENOS_PAGE_SIZE * index), PAMA_FLAG_PRESENT | PAMA_FLAG_KERNEL);
		allocation->references_count++;

		// Quick and dirty fetch the virtual base
		if (index == 0)
		  {
			framebuffer_info.FrameBufferBase = allocation->virtual_start + aligment_offset;
		  }
	  }
  }

  uint64_t stack_physical_address_base;
  uint64_t stack_virtual_address_top = ISENOS_KERNEL_STACK_TOP;
  // One page for the stack
  {
	stack_physical_address_base = pam_find_free_pages (1);
	page_allocation_manager_allocation_t *allocation = pam_add_allocation (
		stack_physical_address_base,
		ISENOS_KERNEL_STACK_BASE,
		PAMA_FLAG_KERNEL | PAMA_FLAG_PRESENT);

	allocation->references_count++;
  }
  printf ("[NEW_STACK] PHYSICAL_BASE = %016"PRIx64 " PHYSICAL_TOP = %016"PRIx64" VIRTUAL_BASE = %016"PRIx64 " VIRTUAL_TOP = %016"PRIx64 " \n",
		  stack_physical_address_base,
		  stack_physical_address_base + ISENOS_PAGE_SIZE,
		  ISENOS_KERNEL_STACK_BASE,
		  ISENOS_KERNEL_STACK_TOP
  );

  // Init the static memory mappings
  vmt_init_static_mappings ();

  // Print allocated physical memory pages
  pam_debug_print ();

  // Generate VirtualMemory tables (to be stored in CR3)
  uint64_t tables_page_physical = vmtm_update (0);

  // Tell the CPU where to find the new pages
  asm ("movq  %0, %%cr3\n"::"r"(tables_page_physical));

  // Move the stack because we are at the top of the main
  __asm__ volatile (
	  "movq  %q0, %%rbp"::"r" (ISENOS_KERNEL_STACK_TOP - 0x10)
	  );
  __asm__ volatile (
	  "movq  %q0, %%rsp"::"r" (ISENOS_KERNEL_STACK_TOP - 0x10)
	  );

  Kmm.memory_setup_complete = 1;
  Kmm.pam_allocations = (void*)PAM_BASE;

  printf ("Looking for ACPI table");

  uint64_t uefi_rsdp = acpi_find_rsdp (ACPI_BASE, ACPI_TOP);

  printf ("Kernel finished initialization");

  init_disks ();

  // Re-enable interrupts
  enable_interrupts ();
  text_output_manager_clear ();
  text_output_manager_add_string ("Welcome to ISENOS v1.1\0");
  text_output_manager_new_line ();
  text_output_manager_add_string ("ring0@isen-os: \0");

  init_pit (1000);
  console_manager_init ();

  k_main_loop ();

  return 0;
}

void k_main_loop ()
{
  int counter = 0;
  while (0 == 0)
	{
	  if (counter > 256)
		{
		  gm_clear ();
		  counter = 0;
		}
	  gm_render ();
	  isenos_programs_e next_program = console_manager_get_next_program ();
	  if (next_program != ISENOS_PROGRAMS_NONE)
		{
		  if (next_program == ISENOS_PROGRAMS_SCROLL)
			{
			  scheduler_start_program (next_program);
			  color_scroll ();
			}
		  else if (next_program == ISENOS_PROGRAMS_SYSINFO)
			{
			  scheduler_start_program (next_program);
			  sys_info ();
			}
		  else if (next_program == ISENOS_PROGRAMS_DONUT)
			{
			  scheduler_start_program (next_program);
			  donut ();
			}
		  else if (next_program == ISENOS_PROGRAMS_MATRIX)
			{
			  scheduler_start_program (next_program);
			  matrix ();
			}
		  // quick and dirty hack
		  console_manager_reset_input ();
		  text_output_manager_clear();
		  text_output_manager_add_string ("Welcome to ISENOS v1.1\0");
		  text_output_manager_new_line ();
		  text_output_manager_add_string ("ring0@isen-os: \0");
		  gm_clear();
		  gm_render();
		}
	  counter++;
	}
}