//
// Created by rfleuryleveso on 20/04/23.
//

#include "acpi.h"
#include "../print.h"
#include "inttypes.h"

typedef struct {
	char signature[8];
	uint8_t checksum;
	char oemid[6];
	uint8_t revision;
	uint32_t rsdt_address;
} rsdp_t;

uint64_t acpi_find_rsdp (uint64_t acpi_base_address, uint64_t acpi_top_address)
{
  printf ("[ACPI] Searching for ACPI_TABLE between  %016"PRIx64" and %016"PRIx64"\n", acpi_base_address, acpi_top_address);
  const char rsdp_signature[8] = {'R', 'S', 'D', ' ', 'P', 'T', 'R', ' '};
  rsdp_t* rsdp = 0x0;

  for (uintptr_t i = acpi_base_address; i < acpi_top_address; i += 16)
	{
	  char *signature = (char *)(uintptr_t)i;
	  int match = 1;
	  for (int j = 0; j < 8; j++)
		{
		  if (signature[j] != rsdp_signature[j])
			{
			  match = 0;
			  break;
			}
		  if (match) {
			  rsdp = (rsdp_t*)signature;

			  // Found valid RSDP
			  break;
			}
		}
	}

  printf ("[ACPI] Found ACPI_TABLE at %016"PRIx64"\n", rsdp);
  return (uint64_t )rsdp;
}
