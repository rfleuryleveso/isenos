//
// Created by rfleuryleveso on 20/04/23.
//

#ifndef _DISK_H_
#define _DISK_H_

#include "inttypes.h"

#define ATA_CONTROL_BASE 0x03F0
#define ATA_IO_BASE_PRIMARY 0x1F0
#define ATA_IO_BASE_SECONDARY 0x1F0

// IO REGISTERS
#define ATA_IOREG_DATA_R 0x0
#define ATA_IOREG_ERROR_R 0x1
#define ATA_IOREG_FEATURES_W 0x1
#define ATA_IOREG_SECTOR_COUNT_RW 0x2
#define ATA_IOREG_SECTOR_NUMBER_RW 0x3
#define ATA_IOREG_CYLINDER_LOW_RW 0x4
#define ATA_IOREG_CYLINDER_HIGH_RW 0x5
#define ATA_IOREG_DRIVE_HEAD_RW 0x6
#define ATA_IOREG_STATUS_R 0x7
#define ATA_IOREG_COMMAND_W 0x7

// CONTROL REGISTERS
#define ATA_CTRLREG_ALTERNATE_STATUS_R 0x0
#define ATA_CTRLREG_DEVICE_CONTROL_W 0x0
#define ATA_CTRLREG_DRIVE_ADDRESS_R 0x1

#define ATA_STAT_ERR  (1 << 0) // Indicates an error occurred. Send a new command to clear it
#define ATA_STAT_DRQ  (1 << 3) // Set when the drive has PIO data to transfer, or is ready to accept PIO data.
#define ATA_STAT_SRV  (1 << 4) // Overlapped Mode Service Request.
#define ATA_STAT_DF   (1 << 5) // Drive Fault Error (does not set ERR).
#define ATA_STAT_RDY  (1 << 6) // Bit is clear when drive is spun down, or after an error. Set otherwise.
#define ATA_STAT_BSY  (1 << 7) // Indicates the drive is preparing to send/receive data (wait for it to clear).
// In case of 'hang' (it never clears), do a software reset.


void init_disks();

#endif //_DISK_H_
