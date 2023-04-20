//
// Created by rfleuryleveso on 20/04/23.
//

#include "disk.h"
#include "../common/ports.h"
#include "../print.h"
#include "../memory/memory_tools.h"

/*
 BSY: a 1 means that the controller is busy executing a command. No register should be accessed (except the digital output register) while this bit is set.
RDY: a 1 means that the controller is ready to accept a command, and the drive is spinning at correct speed..
WFT: a 1 means that the controller detected a write fault.
SKC: a 1 means that the read/write head is in position (seek completed).
DRQ: a 1 means that the controller is expecting data (for a write) or is sending data (for a read). Don't access the data register while this bit is 0.
COR: a 1 indicates that the controller had to correct data, by using the ECC bytes (error correction code: extra bytes at the end of the sector that allows to verify its integrity and, sometimes, to correct errors).
IDX: a 1 indicates the the controller retected the index mark (which is not a hole on hard-drives).
ERR: a 1 indicates that an error occured. An error code has been placed in the error register.
*/


uint8_t identify (uint8_t drive_number)
{
  inb (ATA_IO_BASE_PRIMARY + ATA_IOREG_STATUS_R);
  outb (ATA_IO_BASE_PRIMARY + ATA_IOREG_DRIVE_HEAD_RW, (drive_number << 4) | 0xA0);
  inb (ATA_IO_BASE_PRIMARY + ATA_IOREG_STATUS_R);
  outb (ATA_IO_BASE_PRIMARY + ATA_IOREG_SECTOR_COUNT_RW, 0);
  inb (ATA_IO_BASE_PRIMARY + ATA_IOREG_STATUS_R);
  outb (ATA_IO_BASE_PRIMARY + ATA_IOREG_SECTOR_NUMBER_RW, 0);
  inb (ATA_IO_BASE_PRIMARY + ATA_IOREG_STATUS_R);
  outb (ATA_IO_BASE_PRIMARY + ATA_IOREG_CYLINDER_LOW_RW, 0);
  inb (ATA_IO_BASE_PRIMARY + ATA_IOREG_STATUS_R);
  outb (ATA_IO_BASE_PRIMARY + ATA_IOREG_CYLINDER_HIGH_RW, 0);
  inb (ATA_IO_BASE_PRIMARY + ATA_IOREG_STATUS_R);
  outb (ATA_IO_BASE_PRIMARY + ATA_IOREG_STATUS_R, 0xEC);
  outb (ATA_IO_BASE_PRIMARY + ATA_IOREG_STATUS_R, 0xE7);

  // Read the status port. If it's zero, the drive does not exist.
  uint8_t status = inb (ATA_IO_BASE_PRIMARY + ATA_IOREG_STATUS_R);

  printf ("Waiting for status.\n");
  while (status & ATA_STAT_BSY)
	{
	  uint32_t i = 0;
	  while (i < 0xfffffff)
		{
		  printf ("Printing stuff %d\n", i);
		  i++;
		}
	  printf ("Checking regstat.\n");
	  status = inb (ATA_IO_BASE_PRIMARY + ATA_IOREG_STATUS_R);
	}

  if (status == 0) return 0;

  printf ("Status indicates presence of a drive. Polling while STAT_BSY... ");
  while (status & ATA_STAT_BSY)
	{
	  printf ("\ninb(ATA_PRIMARY_COMM_REGSTAT);... ");
	  status = inb (ATA_IO_BASE_PRIMARY + ATA_IOREG_STATUS_R);
	}
  printf ("Done.\n");

  uint8_t mid = inb (ATA_IO_BASE_PRIMARY + ATA_IOREG_CYLINDER_LOW_RW);
  uint8_t hi = inb (ATA_IO_BASE_PRIMARY + ATA_IOREG_CYLINDER_HIGH_RW);
  if (mid || hi)
	{
	  // The drive is not ATA. (Who knows what it is.)
	  return 0;
	}

  printf ("Waiting for ERR or DRQ.\n");
  // Wait for ERR or DRQ
  while (!(status & (ATA_STAT_ERR | ATA_STAT_DRQ)))
	{
	  status = inb (ATA_IO_BASE_PRIMARY + ATA_IOREG_STATUS_R);
	}

  if (status & ATA_STAT_ERR)
	{
	  // There was an error on the drive. Forget about it.
	  return 0;
	}

  printf ("Reading IDENTIFY structure.\n");
  //uint8_t *buff = kmalloc(40960, 0, NULL);
  uint16_t buff[256];
  for (int i = 0; i < 256; i++)
	{
	  buff[i] = inw (ATA_IO_BASE_PRIMARY + ATA_IOREG_CYLINDER_LOW_RW);
	}
  printf ("Success. Disk is ready to go: %d.\n", buff[0]);
  // We read it!
  return 1;
}

void ata_pio_read48 (uint8_t drive_number, uint64_t LBA, uint16_t sectorcount, uint8_t *target)
{
  // HARD CODE MASTER (for now)
  outb (ATA_IO_BASE_PRIMARY + ATA_IOREG_DRIVE_HEAD_RW, (drive_number << 4) | 0xA0);                     // Select master
  outb (ATA_IO_BASE_PRIMARY + ATA_IOREG_SECTOR_COUNT_RW, (sectorcount >> 8) & 0xFF); // sectorcount high
  outb (ATA_IO_BASE_PRIMARY + ATA_IOREG_SECTOR_NUMBER_RW, (LBA >> 24) & 0xFF);           // LBA4
  outb (ATA_IO_BASE_PRIMARY + ATA_IOREG_CYLINDER_LOW_RW, (LBA >> 32) & 0xFF);          // LBA5
  outb (ATA_IO_BASE_PRIMARY + ATA_IOREG_CYLINDER_HIGH_RW, (LBA >> 40) & 0xFF);           // LBA6
  outb (ATA_IO_BASE_PRIMARY + ATA_IOREG_SECTOR_COUNT_RW, sectorcount & 0xFF);         // sectorcount low
  outb (ATA_IO_BASE_PRIMARY + ATA_IOREG_SECTOR_NUMBER_RW, LBA & 0xFF);                   // LBA1
  outb (ATA_IO_BASE_PRIMARY + ATA_IOREG_CYLINDER_LOW_RW, (LBA >> 8) & 0xFF);           // LBA2
  outb (ATA_IO_BASE_PRIMARY + ATA_IOREG_CYLINDER_HIGH_RW, (LBA >> 16) & 0xFF);           // LBA3
  outb (ATA_IO_BASE_PRIMARY + ATA_IOREG_STATUS_R, 0x24);                   // READ SECTORS EXT


  uint8_t i;
  for (i = 0; i < sectorcount; i++)
	{
	  // POLL!
	  while (1)
		{
		  uint8_t status = inb (ATA_IO_BASE_PRIMARY + ATA_IOREG_STATUS_R);
		  if (status & ATA_STAT_DRQ)
			{
			  // Drive is ready to transfer data!
			  break;
			}
		}
	  // Transfer the data!
	  for (int i = 0; i < 256; i++)
		{
		  target[i] = inw (ATA_IO_BASE_PRIMARY + ATA_IOREG_DATA_R);
		}

	  // target += 256;
	}

}

void init_disks ()
{
  identify (0);
  identify (1);
//  uint8_t sectors[512];
//  memset(sectors, 512, 0);
//  ata_pio_read48 (0, 0xFF, 1, sectors);
//  for (int i = 0; i < 512; i++)
//	{
//	  if (i % 32 == 0)
//		{
//		  printf ("\n");
//		}
//	  printf ("%2x ", sectors[i]);
//	}
//  printf ("\n");
//  ata_pio_read48 (1, 0xFF, 1, sectors);
//  for (int i = 0; i < 512; i++)
//	{
//	  if (i % 32 == 0)
//		{
//		  printf ("\n");
//		}
//	  printf ("%x", sectors[i]);
//	}
//  printf ("\n");
}
