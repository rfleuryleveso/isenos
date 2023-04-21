//
// Created by rfleuryleveso on 21/04/23.
//

#ifndef _TEXT_OUTPUT_MANAGER_H_
#define _TEXT_OUTPUT_MANAGER_H_
#include "stdint.h"
#include "../memory/memory_tools.h"

#define TOM_COLUMNS 60

#define TOM_ROWS 40

extern volatile char graphics_lines[TOM_ROWS][TOM_COLUMNS];



void text_output_manager_clear();
void text_output_manager_new_line();
void text_output_manager_add(char ascii_code);
void text_output_manager_add_string(char* str);
void text_output_manager_remove_char (uint64_t count);

#endif //_TEXT_OUTPUT_MANAGER_H_
