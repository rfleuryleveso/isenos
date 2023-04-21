//
// Created by rfleuryleveso on 21/04/23.
//

#ifndef _CONSOLE_MANAGER_H_
#define _CONSOLE_MANAGER_H_

#include "stdint.h"
#include "../scheduler/scheduler.h"

void console_manager_keyboard_callback(uint8_t scan_code);



// Next program to be started
typedef struct {
	isenos_programs_e next_program;
} console_manager_t;

extern volatile console_manager_t console_manager;

extern volatile uint8_t console_manager_current_line_length;
extern volatile char console_manager_current_line[256];

isenos_programs_e console_manager_get_next_program();
void console_manager_init();
void console_manager_reset_input();
#endif //_CONSOLE_MANAGER_H_
