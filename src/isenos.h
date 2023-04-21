//
// Created by rfleuryleveso on 16/04/23.
//

#include "stdint.h"

#ifndef BLANK_OS_ISENOS_H
#define BLANK_OS_ISENOS_H

#define FALSE 0
#define TRUE 1

typedef int boolean_t;

typedef struct _ELF_INFO {
	// will subtract this value from the Virtual address
	// if zero then physical address is used
	uint64_t VirtualOffset;
	uint64_t PhysicalBase;
	uint64_t PhysicalTop;

	// The idt_entries of the image
	uint64_t Entry;

	// section idt_entries info
	void *SectionHeaders;
	uint64_t SectionHeadersSize;
	uint64_t SectionEntrySize;
	uint64_t StringSectionIndex;
} ELF_INFO;

typedef struct _IBL_MEMORY_MAP_ENTRY {
	// IBL_MEMORY_* Types
	// If type is 0, then this is not allocated
	uint32_t Type;
	uint64_t PhysicalStart;
	uint64_t NumberOfPages;
	uint64_t Size; // Size in bytes
	///
	/// Attributes of the memory region that describe the bit mask of capabilities
	/// for that memory region, and not necessarily the current settings for that
	/// memory region.
	///
	uint64_t Attribute;
} IBL_MEMORY_MAP_ENTRY;

typedef struct _IBL_FRAMEBUFFERINFO {
	uint64_t HorizontalResolution;
	uint64_t VerticalResolution;
	uint64_t PixelFormat;
	uint64_t FrameBufferBase;
	uint64_t FrameBufferSize;
	uint64_t PixelsPerScanLine;
} IBL_FRAMEBUFFER_INFO;

typedef struct _KERNEL_MEMORY_MAPPING {
	uint64_t PhysicalBase;
	uint64_t VirtualBase;
	uint8_t PAGES;
} KERNEL_MEMORY_MAPPING;

typedef struct _IBL_ISENOSDATA {

	IBL_MEMORY_MAP_ENTRY *MemoryMap;   // Pointer to the MemoryMap
	uint32_t MemoryMapSize; // Fixed to 64, but always interesting to pass to the OS
	uint64_t HighestMemoryAddress; // Highest used physical memory address
	IBL_FRAMEBUFFER_INFO *FrameBufferInfo;  // Pointer to the FrameBufferInfo
	ELF_INFO *ElfInfo;

	uint64_t VmmStart; // Start of the VirtualMemoryMap, can be reused once a new virtual address scheme has been setup
	uint64_t VmmEnd; // End of the VirtualMemoryMap

	uint64_t KernelStart; // Physical address of the kernel mapping
	uint64_t KernelEnd;

	KERNEL_MEMORY_MAPPING KernelMemoryMappings[32]; // Memory mappings of the kernel

	uint64_t EfiTablePointer;
} IBL_ISENOS_DATA;

static char ISEN_OS_SPLASH[] = { 0x0A, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x2C, 0x2D, 0x2D, 0x2D, 0x2D, 0x2E, 0x2E, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x20, 0x20,
								 0x20, 0x2C, 0x2D, 0x2D, 0x2D, 0x2C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x2F, 0x20, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x20, 0x2E, 0x2D,
								 0x2D, 0x2E, 0x2D, 0x2D, 0x2E, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x2C, 0x60, 0x2D, 0x2D,
								 0x2E, 0x27, 0x20, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2F, 0x20, 0x20,
								 0x20, 0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3A, 0x20, 0x2F, 0x20, 0x20, 0x2F, 0x20,
								 0x20, 0x20, 0x20, 0x27, 0x2E, 0x20, 0x20, 0x0A, 0x7C, 0x20, 0x20, 0x20, 0x3A, 0x20,
								 0x20, 0x3A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x2C, 0x2D, 0x2D, 0x2D, 0x2C, 0x20, 0x20, 0x2E, 0x20, 0x20, 0x20, 0x2F, 0x20,
								 0x20, 0x20, 0x3B, 0x2E, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x3A, 0x20, 0x20, 0x2F, 0x60,
								 0x2E, 0x20, 0x2F, 0x20, 0x20, 0x0A, 0x3A, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x27,
								 0x20, 0x20, 0x2E, 0x2D, 0x2D, 0x2E, 0x2D, 0x2D, 0x2E, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2C, 0x2D, 0x2B, 0x2D, 0x2E,
								 0x20, 0x2F, 0x20, 0x20, 0x7C, 0x2E, 0x20, 0x20, 0x20, 0x3B, 0x20, 0x20, 0x20, 0x2F,
								 0x20, 0x20, 0x60, 0x20, 0x3B, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x7C, 0x2D, 0x2D, 0x60,
								 0x20, 0x20, 0x20, 0x0A, 0x7C, 0x20, 0x20, 0x20, 0x3A, 0x20, 0x20, 0x7C, 0x20, 0x2F,
								 0x20, 0x20, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x27, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2C,
								 0x2D, 0x2D, 0x2D, 0x2E, 0x20, 0x20, 0x2C, 0x2D, 0x2D, 0x2E, 0x27, 0x7C, 0x27, 0x20,
								 0x20, 0x20, 0x7C, 0x3B, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x3B, 0x20, 0x5C, 0x20,
								 0x3B, 0x20, 0x7C, 0x20, 0x20, 0x3A, 0x20, 0x20, 0x3B, 0x5F, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x0A, 0x27, 0x20, 0x20, 0x20, 0x27, 0x20, 0x20, 0x3B, 0x7C, 0x20, 0x20, 0x3A,
								 0x20, 0x20, 0x2F, 0x60, 0x2E, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x5C, 0x7C, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x2C, 0x22, 0x27, 0x20,
								 0x7C, 0x7C, 0x20, 0x20, 0x20, 0x3A, 0x20, 0x20, 0x7C, 0x20, 0x3B, 0x20, 0x7C, 0x20,
								 0x27, 0x5C, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x20, 0x20, 0x60, 0x2E, 0x20, 0x20, 0x0A,
								 0x7C, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x7C, 0x7C, 0x20, 0x20, 0x3A, 0x20, 0x20,
								 0x3B, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x20, 0x20, 0x2F, 0x20,
								 0x20, 0x7C, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x2F, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x2E,
								 0x20, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x27, 0x20, 0x27, 0x20, 0x27, 0x20, 0x3A, 0x20,
								 0x60, 0x2D, 0x2D, 0x2D, 0x2D, 0x2E, 0x20, 0x20, 0x20, 0x5C, 0x20, 0x0A, 0x27, 0x20,
								 0x20, 0x20, 0x3A, 0x20, 0x20, 0x3B, 0x20, 0x5C, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x20,
								 0x20, 0x60, 0x2E, 0x20, 0x2E, 0x20, 0x20, 0x20, 0x20, 0x27, 0x20, 0x2F, 0x20, 0x7C,
								 0x20, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x27, 0x20, 0x20,
								 0x20, 0x3B, 0x20, 0x20, 0x5C, 0x3B, 0x20, 0x2F, 0x20, 0x20, 0x7C, 0x20, 0x5F, 0x5F,
								 0x20, 0x5C, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x7C, 0x20, 0x0A, 0x7C, 0x20, 0x20, 0x20,
								 0x7C, 0x20, 0x20, 0x27, 0x20, 0x20, 0x60, 0x2D, 0x2D, 0x2D, 0x2D, 0x2E, 0x20, 0x20,
								 0x20, 0x5C, 0x27, 0x20, 0x20, 0x20, 0x3B, 0x20, 0x20, 0x20, 0x2F, 0x7C, 0x20, 0x20,
								 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x7C, 0x2F, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x20,
								 0x5C, 0x20, 0x20, 0x27, 0x2C, 0x20, 0x20, 0x2F, 0x20, 0x2F, 0x20, 0x20, 0x2F, 0x60,
								 0x2D, 0x2D, 0x27, 0x20, 0x20, 0x2F, 0x20, 0x0A, 0x27, 0x20, 0x20, 0x20, 0x3A, 0x20,
								 0x20, 0x7C, 0x20, 0x2F, 0x20, 0x20, 0x2F, 0x60, 0x2D, 0x2D, 0x27, 0x20, 0x20, 0x2F,
								 0x27, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x2F, 0x20, 0x7C, 0x20, 0x20, 0x20, 0x7C,
								 0x20, 0x7C, 0x2D, 0x2D, 0x27, 0x20, 0x20, 0x20, 0x20, 0x3B, 0x20, 0x20, 0x20, 0x3A,
								 0x20, 0x20, 0x20, 0x20, 0x2F, 0x20, 0x27, 0x2D, 0x2D, 0x27, 0x2E, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x2F, 0x20, 0x20, 0x0A, 0x3B, 0x20, 0x20, 0x20, 0x7C, 0x2E, 0x27, 0x20,
								 0x27, 0x2D, 0x2D, 0x27, 0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2F, 0x20, 0x7C, 0x20,
								 0x20, 0x20, 0x3A, 0x20, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x20, 0x7C, 0x2F, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x20, 0x20, 0x20, 0x5C, 0x20,
								 0x2E, 0x27, 0x20, 0x20, 0x20, 0x20, 0x60, 0x2D, 0x2D, 0x27, 0x2D, 0x2D, 0x2D, 0x27,
								 0x20, 0x20, 0x20, 0x0A, 0x27, 0x2D, 0x2D, 0x2D, 0x27, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x60, 0x2D, 0x2D, 0x27, 0x2D, 0x2D, 0x2D, 0x27, 0x20, 0x20, 0x20, 0x5C, 0x20, 0x20,
								 0x20, 0x5C, 0x20, 0x20, 0x2F, 0x27, 0x2D, 0x2D, 0x2D, 0x27, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60, 0x2D, 0x2D, 0x2D, 0x60, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60, 0x2D, 0x2D, 0x2D,
								 0x2D, 0x27, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
								 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xA };
#endif //BLANK_OS_ISENOS_H
