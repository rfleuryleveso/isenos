//
// Created by rfleuryleveso on 21/04/23.
//

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "stdint.h"
#include "../memory/page_allocation/page_allocation_manager_allocation.h"

#define PROGRAM_DESCRIPTORS_COUNT 32

#define PROGRAM_STATUS_ALLOCATED (1 << 1)
#define PROGRAM_STATUS_RUNNING (1 << 2)
#define PROGRAM_STATUS_KERNEL (1 << 3)
#define PROGRAM_STATUS_STOPPING (1 << 4)

#define SCHEDULER_KBD_FLAGS_CTRL_PRESSED (1 << 1)
#define SCHEDULER_KBD_FLAGS_C_PRESSED (1 << 1)

typedef enum {
	ISENOS_PROGRAMS_NONE,
	ISENOS_PROGRAMS_KERNEL,
	ISENOS_PROGRAMS_SYSINFO,
	ISENOS_PROGRAMS_SCROLL,
	ISENOS_PROGRAMS_DONUT,
	ISENOS_PROGRAMS_MATRIX,
	ISENOS_PROGRAMS_DOOM // :(
} isenos_programs_e;

typedef struct {
	isenos_programs_e program;
	uint64_t program_id;
	uint64_t status;

	page_allocation_manager_allocation_t* stack_allocation;
	page_allocation_manager_allocation_t* heap;


	uint64_t ax;
	uint64_t bx;
	uint64_t cx;
	uint64_t dx;
	uint64_t si;
	uint64_t di;
	uint64_t bp;
	uint64_t sp;
	uint64_t r8;
	uint64_t r9;
	uint64_t r10;
	uint64_t r11;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t ip;
	uint64_t fl;

} scheduler_program_descriptor_t;

typedef struct {
	scheduler_program_descriptor_t* current_program;
	scheduler_program_descriptor_t program_descriptors[PROGRAM_DESCRIPTORS_COUNT];

	uint8_t keyboard_flags;
} scheduler_t;

extern scheduler_t kernel_scheduler;

/**
 * Start a program, setup the heap and stack
 * @param program
 */
scheduler_program_descriptor_t* scheduler_start_program(isenos_programs_e program);

uint8_t scheduler_program_should_stop();

void scheduler_tick();

void scheduler_handle_scancode(uint64_t scancode);

#endif //_SCHEDULER_H_
