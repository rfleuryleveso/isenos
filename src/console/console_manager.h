//
// Created by rfleuryleveso on 21/04/23.
//

#ifndef _CONSOLE_MANAGER_H_
#define _CONSOLE_MANAGER_H_

#include "stdint.h"

void console_manager_keyboard_callback(uint8_t scan_code);

extern volatile uint8_t console_manager_current_line_length;
extern volatile char console_manager_current_line[256];

#endif //_CONSOLE_MANAGER_H_
