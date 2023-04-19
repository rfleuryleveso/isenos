//
// Created by rfleuryleveso on 18/04/23.
//

#ifndef _VM_H_
#define _VM_H_

#include "stdint.h"

#pragma pack(1)

#define BIT(n) (1<<(n))

typedef union {
	struct {
		uint64_t Present: 1;  // 0 = Not present in memory,
		//   1 = Present in memory
		uint64_t ReadWrite: 1;  // 0 = Read-Only, 1= Read/Write
		uint64_t UserSupervisor: 1;  // 0 = Supervisor, 1=User
		uint64_t WriteThrough: 1;  // 0 = Write-Back caching,
		//   1 = Write-Through caching
		uint64_t CacheDisabled: 1;  // 0 = Cached, 1=Non-Cached
		uint64_t Accessed: 1;  // 0 = Not accessed,
		//   1 = Accessed (set by CPU)
		uint64_t Reserved: 1;  // Reserved
		uint64_t MustBeZero: 2;  // Must Be Zero
		uint64_t Available: 3;  // Available for use by system software
		uint64_t PageTableBaseAddress: 40; // Page Table Base Address
		uint64_t AvabilableHigh: 11; // Available for use by system software
		uint64_t Nx: 1;  // No Execute bit
	} Bits;
	uint64_t uint64;
} PAGE_MAP_AND_DIRECTORY_POINTER;

//
// Page Table Entry 4KB
//
typedef union {
	struct {
		uint64_t Present: 1;  // 0 = Not present in memory,
		//   1 = Present in memory
		uint64_t ReadWrite: 1;  // 0 = Read-Only, 1= Read/Write
		uint64_t UserSupervisor: 1;  // 0 = Supervisor, 1=User
		uint64_t WriteThrough: 1;  // 0 = Write-Back caching,
		//   1 = Write-Through caching
		uint64_t CacheDisabled: 1;  // 0 = Cached, 1=Non-Cached
		uint64_t Accessed: 1;  // 0 = Not accessed,
		//   1 = Accessed (set by CPU)
		uint64_t Dirty: 1;  // 0 = Not Dirty, 1 = written by
		//   processor on access to page
		uint64_t PAT: 1;  //
		uint64_t Global: 1;  // 0 = Not global page, 1 = global page
		//   TLB not cleared on CR3 write
		uint64_t Available: 3;  // Available for use by system software
		uint64_t PageTableBaseAddress: 40; // Page Table Base Address
		uint64_t AvabilableHigh: 11; // Available for use by system software
		uint64_t Nx: 1;  // 0 = Execute Code,
		//   1 = No Code Execution
	} Bits;
	uint64_t uint64;
} PAGE_TABLE_4K_ENTRY;

//
// Page Table Entry 2MB
//
typedef union {
	struct {
		uint64_t Present: 1;  // 0 = Not present in memory,
		//   1 = Present in memory
		uint64_t ReadWrite: 1;  // 0 = Read-Only, 1= Read/Write
		uint64_t UserSupervisor: 1;  // 0 = Supervisor, 1=User
		uint64_t WriteThrough: 1;  // 0 = Write-Back caching,
		//   1=Write-Through caching
		uint64_t CacheDisabled: 1;  // 0 = Cached, 1=Non-Cached
		uint64_t Accessed: 1;  // 0 = Not accessed,
		//   1 = Accessed (set by CPU)
		uint64_t Dirty: 1;  // 0 = Not Dirty, 1 = written by
		//   processor on access to page
		uint64_t MustBe1: 1;  // Must be 1
		uint64_t Global: 1;  // 0 = Not global page, 1 = global page
		//   TLB not cleared on CR3 write
		uint64_t Available: 3;  // Available for use by system software
		uint64_t PAT: 1;  //
		uint64_t MustBeZero: 8;  // Must be zero;
		uint64_t PageTableBaseAddress: 31; // Page Table Base Address
		uint64_t AvabilableHigh: 11; // Available for use by system software
		uint64_t Nx: 1;  // 0 = Execute Code,
		//   1 = No Code Execution
	} Bits;
	uint64_t uint64;
} PAGE_TABLE_ENTRY;

#pragma pack()

#define IA32_PG_P       BIT(0)
#define IA32_PG_RW      BIT(1)
#define IA32_PG_U       BIT(2)
#define IA32_PG_WT      BIT(3)
#define IA32_PG_CD      BIT(4)
#define IA32_PG_A       BIT(5)
#define IA32_PG_D       BIT(6)
#define IA32_PG_PS      BIT(7)
#define IA32_PG_PAT_2M  BIT(12)
#define IA32_PG_PAT_4K  IA32_PG_PS
#define IA32_PG_PMNT    BIT(62)
#define IA32_PG_NX      BIT(63)

#define PAGING_PAE_INDEX_MASK  0x1FF

#define PAGING_4K_ADDRESS_MASK_64  0x000FFFFFFFFFF000ull
#define PAGING_2M_ADDRESS_MASK_64  0x000FFFFFFFE00000ull
#define PAGING_1G_ADDRESS_MASK_64  0x000FFFFFC0000000ull

#define PAGING_L1_ADDRESS_SHIFT  12
#define PAGING_L2_ADDRESS_SHIFT  21
#define PAGING_L3_ADDRESS_SHIFT  30
#define PAGING_L4_ADDRESS_SHIFT  39

#define PAGING_PML4E_NUMBER  4

#endif //_VM_H_
