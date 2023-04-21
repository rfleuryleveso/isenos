//
// Created by rfleuryleveso on 18/04/23.
//

#include "virtual_memory_tables_manager.h"
#include "static_mappings.h"
#include "../memory_tools.h"

uint8_t bitmap_get_bit (uint64_t *bitmap, uint64_t index)
{
  uint8_t bitmap_index = (index & ~0x3F) >> 6;
  uint8_t bitmap_shift = index & 0x3F;
  return (bitmap[bitmap_index] >> bitmap_shift) & 0x1;
}

void bitmap_set_bit (uint64_t *bitmap, uint64_t index, uint8_t value)
{
  uint8_t bitmap_index = (index & ~0x3F) >> 6;
  uint8_t bitmap_shift = index & 0x3F;
  if (value == 0)
	{
	  bitmap[bitmap_index] &= ~((uint64_t)1 << bitmap_shift);
	}
  else
	{
	  bitmap[bitmap_index] |= (uint64_t)1 << bitmap_shift;
	}
}

void vmt_va_2_indexes (uint64_t virtual_address, uint64_t *pml4index, uint64_t *pdptindex, uint64_t *pdindex)
{
  *pml4index = virtual_address >> PAGING_L4_ADDRESS_SHIFT & PAGING_PAE_INDEX_MASK;
  *pdptindex = virtual_address >> PAGING_L3_ADDRESS_SHIFT & PAGING_PAE_INDEX_MASK;
  *pdindex = virtual_address >> PAGING_L2_ADDRESS_SHIFT & PAGING_PAE_INDEX_MASK;
}

/**
 * Returns the index of the PDPTable in the list of PDP tables
 * @param filled_pml4
 * @param pml4_index
 * @return
 */
uint16_t vmt_get_pdpt_index (uint64_t *filled_pml4, uint16_t pml4_index)
{
  int index = 0;
  for (int i = 0; i < 512; i++)
	{
	  if (pml4_index == i)
		{
		  return index;
		}
	  else
		{
		  if (bitmap_get_bit (filled_pml4, i))
			{
			  index++;
			}
		}
	}
}
/**
 * Returns the index of the PDTable in the list of PD tables
 * @param filled_pml4
 * @param pml4_index
 * @return
 */
uint16_t vmt_get_pdt_index (uint64_t *filled_pml4, uint64_t *filled_pdp, uint16_t pml4_index, uint16_t pdp_index)
{
  int index = 0;

  // Each PML4 entry gives 512 PDP tables
  for (int i = 0; i < 512; i++)
	{
	  if (bitmap_get_bit (filled_pml4, i))
		{
		  // Each PDP entry gives 512 PD, only if the the PDP entry is filled
		  for (int j = 0; j < 512; j++)
			{
			  if (pml4_index == i && pdp_index == j)
				{
				  return index;
				}
			  else if (bitmap_get_bit (filled_pdp, i * 512 + j))
				{
				  index++;
				}
			}
		}
	}
}

void vmt_compute_tables_counts (uint64_t *pd_table_count, uint64_t *pdp_table_count,
								uint64_t *filled_pml4,
								uint64_t *filled_pdp)
{
  // Find all virtual adresses in use
  page_allocation_manager_allocation_t *base_allocation = Kmm.memory_setup_complete ? Kmm.pam_allocations
																						   : (void *)Kmm.pam_base;
  // Find out the number of used

  printf ("[VIRTUAL_MEMORY_TABLES_MANAGER][DEBUG] Computing table counts for %d allocations @ 0x%x\n", Kmm.pam_allocations_count, base_allocation);


  // For each pam idt_entries, set up the corresponding PML4 idt_entries
  for (int pam_allocation_index = 0; pam_allocation_index < Kmm.pam_allocations_count; pam_allocation_index++)
	{
	  page_allocation_manager_allocation_t *allocation = &base_allocation[pam_allocation_index];
	  if ((allocation->flags & PAMA_FLAG_PRESENT) == PAMA_FLAG_PRESENT)
		{
		  uint64_t virtual_address = allocation->virtual_start;
		  uint64_t va_pml4_index, va_pdpt_index, va_pd_index;
		  vmt_va_2_indexes (virtual_address, &va_pml4_index, &va_pdpt_index, &va_pd_index);
		  bitmap_set_bit (filled_pml4, va_pml4_index, 1);
		  bitmap_set_bit (filled_pdp, va_pml4_index * 512 + va_pdpt_index, 1);
		}
	}

  printf ("STATIC MEMORY MAPPINGS\n");
  for (int static_mapping_index = 0; static_mapping_index < VMT_STATIC_MAPPING_COUNT; static_mapping_index++)
	{
	  struct VMT_STATIC_MAPPING mapping = static_mappings[static_mapping_index];
	  printf ("MAPPING %d PHYSICAL_ADDRESS = %016"PRIx64" VIRTUAL_ADDRESS = %016"PRIx64" PAGES = %d ENABLED = %d\n", static_mapping_index, mapping.physical_address, mapping.virtual_address, mapping.pages, mapping.enabled);

	  if (mapping.enabled)
		{
		  for (int static_mapping_page = 0; static_mapping_page < mapping.pages; static_mapping_page++)
			{
			  uint64_t physical_address = mapping.physical_address + ISENOS_PAGE_SIZE * static_mapping_page;
			  uint64_t virtual_address = mapping.virtual_address + ISENOS_PAGE_SIZE * static_mapping_page;
			  uint64_t va_pml4_index, va_pdpt_index, va_pd_index;
			  vmt_va_2_indexes (virtual_address, &va_pml4_index, &va_pdpt_index, &va_pd_index);
			  bitmap_set_bit (filled_pml4, va_pml4_index, 1);
			  bitmap_set_bit (filled_pdp, va_pml4_index * 512 + va_pdpt_index, 1);

			}
		}
	}


  // Compute
  for (int pml4index = 0; pml4index < 512; pml4index++)
	{
	  if (bitmap_get_bit (filled_pml4, pml4index))
		{
		  (*pdp_table_count)++;
		  for (int pdptindex = 0; pdptindex < 512; pdptindex++)
			{
			  if (bitmap_get_bit (filled_pdp, pml4index * 512 + pdptindex))
				{
				  (*pd_table_count)++;
				}
			}
		}

	}
}

/**
 *
 * @param virtual_address
 * @param physical_address
 * @param pml4_table_base
 * @param pml4_table_top
 * @param pdp_table_base
 * @param pdp_table_top
 * @param pd_table_base
 * @param pd_table_top
 * @param filled_pml4
 * @param filled_pdp
 * @param offset Offset for translation from VMT pointers to real memory addresses
 */
void vmt_populate_page_table_for_address (
	uint64_t virtual_address,
	uint64_t physical_address,
	void *pml4_table_base,
	void *pml4_table_top,
	void *pdp_table_base,
	void *pdp_table_top,
	void *pd_table_base,
	void *pd_table_top,

	uint8_t filled_pml4[512],
	uint8_t filled_pdp[512][512],

	int64_t offset
)
{
  uint64_t va_pml4_index, va_pdp_index, va_pd_index;
  vmt_va_2_indexes (virtual_address, &va_pml4_index, &va_pdp_index, &va_pd_index);

  PAGE_MAP_AND_DIRECTORY_POINTER *pml4_table = pml4_table_base;
  PAGE_MAP_AND_DIRECTORY_POINTER *pml4_entry = &pml4_table[va_pml4_index];

  uint64_t pdp_table_index = vmt_get_pdpt_index (filled_pml4, va_pml4_index) * 512;
  uint64_t pd_table_index = vmt_get_pdt_index (filled_pml4, filled_pdp, va_pml4_index, va_pdp_index) * 512;

  uint64_t pdp_index = pdp_table_index + va_pdp_index;
  uint64_t pd_index = pd_table_index + va_pd_index;
  printf ("VA = %016"PRIx64" PA = %016"PRIx64" PML4[%d] PDP[%d + %d = %d] PD[%d + %d = %d]\n", virtual_address, physical_address, va_pml4_index, pdp_table_index, va_pdp_index, pdp_index, pd_table_index, va_pd_index, pd_index);

  PAGE_MAP_AND_DIRECTORY_POINTER *pdp_table = pdp_table_base;
  PAGE_MAP_AND_DIRECTORY_POINTER *pdp_entry = &pdp_table[pdp_index];

  PAGE_TABLE_ENTRY *pd_table = pd_table_base;
  PAGE_TABLE_ENTRY *pd_entry = &pd_table[pd_index];

  pml4_entry->uint64 = ((uint64_t)pdp_entry + offset) & 0x000FFFFFFFFFF000ull;
  pml4_entry->Bits.Present = 1;
  pml4_entry->Bits.ReadWrite = 1;
  pml4_entry->Bits.UserSupervisor = 1;

  pdp_entry->uint64 = ((uint64_t)(uint64_t)pd_entry + offset) & 0x000FFFFFFFFFF000ull;
  pdp_entry->Bits.MustBeZero = 0;
  pdp_entry->Bits.Present = 1;
  pdp_entry->Bits.ReadWrite = 1;
  pdp_entry->Bits.UserSupervisor = 1;

  pd_entry->uint64 = physical_address & PAGING_2M_ADDRESS_MASK_64;
  pd_entry->Bits.Present = 1;
  pd_entry->Bits.ReadWrite = 1;
  pd_entry->Bits.UserSupervisor = 1;
  pd_entry->Bits.MustBe1 = 1;
  printf (">	 PML4(%016"PRIx64") = %016"PRIx64"  PDP(%016"PRIx64") = %016"PRIx64"  PD(%016"PRIx64") = %016"PRIx64"\n", pml4_entry, *pml4_entry, pdp_entry, *pdp_entry, pd_entry, *pd_entry);
}

void vmt_populate_page_table (void *base, uint64_t pdp_table_count, uint64_t pd_table_count, uint64_t *filled_pml4,
							  uint64_t *filled_pdp, int64_t offset)
{


  // Find all virtual adresses in use
  page_allocation_manager_allocation_t *base_allocation = Kmm.memory_setup_complete ? Kmm.pam_allocations
																						   : (void *)Kmm.pam_base;
  printf ("[VIRTUAL_MEMORY_TABLES_MANAGER][DEBUG] Populating PML4 table, base = 0x%x\n", base);

  // Find out the number of used
  void *pml4_table_base = base;
  void *pml4_table_top = base + sizeof (PAGE_MAP_AND_DIRECTORY_POINTER) * 512;

  uint64_t pdp_table_size = sizeof (PAGE_MAP_AND_DIRECTORY_POINTER) * 512 * pdp_table_count;
  void *pdp_table_base = pml4_table_top;
  void *pdp_table_top = (void *)((uint64_t)pdp_table_base + pdp_table_size);
  printf ("[VIRTUAL_MEMORY_TABLES_MANAGER][DEBUG] PDP Table size = 0x%x\n", pdp_table_size);

  uint64_t pd_table_size = sizeof (PAGE_MAP_AND_DIRECTORY_POINTER) * 512 * pd_table_count;
  void *pd_table_base = pdp_table_top;
  void *pd_table_top = (void *)((uint64_t)pd_table_base + pd_table_size);
  printf ("[VIRTUAL_MEMORY_TABLES_MANAGER][DEBUG] PD Table size = 0x%x\n", pd_table_size);

  printf ("[VIRTUAL_MEMORY_TABLES_MANAGER][DEBUG] pdp_table_count 	=  %016"PRIx64"  	pd_table_count 		=  %016"PRIx64"\n", pdp_table_count, pd_table_count);
  printf ("[VIRTUAL_MEMORY_TABLES_MANAGER][DEBUG] pml4_table_base 	=  %016"PRIx64"  	pml4_table_top 		=  %016"PRIx64"\n", pml4_table_base, pml4_table_top);
  printf ("[VIRTUAL_MEMORY_TABLES_MANAGER][DEBUG] pdp_table_base 	=  %016"PRIx64"  	pdp_table_top 		=  %016"PRIx64"\n", pdp_table_base, pdp_table_top);
  printf ("[VIRTUAL_MEMORY_TABLES_MANAGER][DEBUG] pd_table_base 	=  %016"PRIx64"  	pd_table_top 		=  %016"PRIx64"\n", pd_table_base, pd_table_top);


  // For each pam idt_entries, set up the corresponding PML4 idt_entries
  for (int pam_allocation_index = 0; pam_allocation_index < Kmm.pam_allocations_count; pam_allocation_index++)
	{
	  page_allocation_manager_allocation_t *allocation = &base_allocation[pam_allocation_index];
	  if ((allocation->flags & PAMA_FLAG_PRESENT) == PAMA_FLAG_PRESENT)
		{
		  uint64_t virtual_address = allocation->virtual_start;
		  uint64_t physical_address = allocation->physical_start;
		  vmt_populate_page_table_for_address (virtual_address, physical_address, pml4_table_base, pml4_table_top, pdp_table_base, pdp_table_top, pd_table_base, pd_table_top, filled_pml4, filled_pdp, offset);
		}
	}

  printf ("STATIC MEMORY MAPPINGS\n");
  for (int static_mapping_index = 0; static_mapping_index < VMT_STATIC_MAPPING_COUNT; static_mapping_index++)
	{
	  struct VMT_STATIC_MAPPING mapping = static_mappings[static_mapping_index];
	  printf ("MAPPING %d PHYSICAL_ADDRESS = %016"PRIx64" VIRTUAL_ADDRESS = %016"PRIx64" PAGES = %d ENABLED = %d\n", static_mapping_index, mapping.physical_address, mapping.virtual_address, mapping.pages, mapping.enabled);
	  if (mapping.enabled)
		{
		  for (int static_mapping_page = 0; static_mapping_page < mapping.pages; static_mapping_page++)
			{
			  uint64_t physical_address = mapping.physical_address + ISENOS_PAGE_SIZE * static_mapping_page;
			  uint64_t virtual_address = mapping.virtual_address + ISENOS_PAGE_SIZE * static_mapping_page;
			  vmt_populate_page_table_for_address (virtual_address, physical_address, pml4_table_base, pml4_table_top, pdp_table_base, pdp_table_top, pd_table_base, pd_table_top, filled_pml4, filled_pdp, offset);
			}
		}
	}
  printf ("STATIC MEMORY MAPPINGS finished\n");
}

uint64_t vmtm_update (uint8_t identity_mapped_memory)
{
  uint64_t bitmaps_heap = pam_find_free_pages (1);
  page_allocation_manager_allocation_t *bitmap_heap_allocation = pam_add_allocation (
	  bitmaps_heap,
	  VMT_BITMAP_BASE, PAMA_FLAG_PRESENT | PAMA_FLAG_VMT);
  bitmap_heap_allocation->references_count = 1;

  uint64_t pd_table_count = 0, pdp_table_count = 0;

  // We need 512 bits
  // (512)/64 = 8 entries
  uint64_t *filled_pml4 = (uint64_t *)bitmaps_heap; // 8 * sizeof(uint64_t) bytes

  // We need 512 * 512 bits
  // (512*512)/64 = 64 entries = 8*8 entries
  uint64_t *filled_pdp = filled_pml4 + 8 * 8; // 8 * 8 * sizeof(uint64_t) bytes

  memset ((void *)filled_pdp, 8 * 8 * sizeof (uint64_t), 0);
  memset ((void *)filled_pml4, 8 * sizeof (uint64_t), 0);

  vmt_compute_tables_counts (&pd_table_count, &pdp_table_count, filled_pml4, filled_pdp);

  uint64_t allocated_size =
	  sizeof (PAGE_MAP_AND_DIRECTORY_POINTER) * 512
	  + sizeof (PAGE_TABLE_4K_ENTRY) * 512 * pdp_table_count
	  + sizeof (PAGE_TABLE_ENTRY) * 512 * pd_table_count;

  uint8_t pages_count =
	  allocated_size < ISENOS_PAGE_SIZE ? 1 : ceil ((float)((double)allocated_size / ISENOS_PAGE_SIZE));

  printf ("[VIRTUAL_MEMORY_TABLES_MANAGER] pd_table_count = %d, pdp_table_count = %d, allocated_size = %d pages_count = %d\n", pd_table_count, pdp_table_count, allocated_size, pages_count);

  // Find enough pages for the VMT
  uint64_t tables_page_physical = pam_find_free_pages (pages_count);

  if (tables_page_physical == 0)
	{
	  printf ("[VIRTUAL_MEMORY_TABLES_MANAGER] Could not find enough pages pages_count = %d\n", pages_count);

	  return 0;
	}

  for (int page_index; page_index < pages_count; page_index++)
	{
	  page_allocation_manager_allocation_t *table_page_allocation = pam_add_allocation (
		  tables_page_physical + (ISENOS_PAGE_SIZE * page_index),
		  VMT_BASE + (ISENOS_PAGE_SIZE * page_index), PAMA_FLAG_PRESENT | PAMA_FLAG_VMT);

	  table_page_allocation->references_count = 1;
	}



  // Find the virtual address of the table
  uint64_t tables_page_virtual = mem_phys_to_virt (tables_page_physical);
  printf ("[VIRTUAL_MEMORY_TABLES_MANAGER] identity_mapped_memory = %d, tables_page_physical = %016"PRIx64 " tables_page_virtual = %016"PRIx64 " \n", identity_mapped_memory, tables_page_physical, tables_page_virtual);

  memset((void*)tables_page_physical, allocated_size, 0);

  // Fill the tables
  vmt_populate_page_table ((void *)tables_page_physical, pdp_table_count, pd_table_count, filled_pml4, filled_pdp, 0);
  printf ("[VIRTUAL_MEMORY_TABLES_MANAGER] VMT = %016"PRIx64 " \n", tables_page_physical);





  bitmap_heap_allocation->references_count = 0;

  return tables_page_physical;
}
