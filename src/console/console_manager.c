//
// Created by rfleuryleveso on 21/04/23.
//

#include "console_manager.h"
#include "../print.h"
#include "../graphics/text_output_manager.h"

volatile char console_manager_current_line[256];
volatile uint8_t console_manager_current_line_length;
volatile console_manager_t console_manager;

void console_manager_keyboard_callback (uint8_t scan_code)
{
  char kbd_FR[128] =
	  {
		  0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
		  '\t', /* <-- Tab */
		  'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
		  0, /* <-- control key */
		  'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', '\'', '`', 0, '\\', 'z', 'w', 'c', 'v', 'b', 'n', 'm', ',',
		  '.', '/', 0,
		  '*',
		  0,  /* Alt */
		  ' ',  /* Space bar */
		  0,  /* Caps lock */
		  0,  /* 59 - F1 key ... > */
		  0, 0, 0, 0, 0, 0, 0, 0,
		  0,  /* < ... F10 */
		  0,  /* 69 - Num lock*/
		  0,  /* Scroll Lock */
		  0,  /* Home key */
		  0,  /* Up Arrow */
		  0,  /* Page Up */
		  '-',
		  0,  /* Left Arrow */
		  0,
		  0,  /* Right Arrow */
		  '+',
		  0,  /* 79 - End key*/
		  0,  /* Down Arrow */
		  0,  /* Page Down */
		  0,  /* Insert Key */
		  0,  /* Delete Key */
		  0, 0, 0,
		  0,  /* F11 Key */
		  0,  /* F12 Key */
		  0,  /* All other keys are undefined */
	  };
  if ((scan_code >= 2 && scan_code <= 13) || (scan_code >= 16 && scan_code <= 25)
	  || (scan_code >= 30 && scan_code <= 53) || scan_code == 74 || scan_code == 78)
	{
	  if (console_manager_current_line_length > 255)
		{
		  return;
		}
	  console_manager_current_line[console_manager_current_line_length] = kbd_FR[scan_code];
	  text_output_manager_add (console_manager_current_line[console_manager_current_line_length]);
	  console_manager_current_line_length++;
	}
  else if (scan_code == 0x9c)
	{
	  text_output_manager_new_line ();


	  if (str_cmp (console_manager_current_line, "scroll") == 0)
		{

		  console_manager.next_program = ISENOS_PROGRAMS_SCROLL;
		  text_output_manager_add_string ("Starting scroll..");
		}
	  else if (str_cmp (console_manager_current_line, "sysinfo") == 0)
		{
		  console_manager.next_program = ISENOS_PROGRAMS_SYSINFO;
		  text_output_manager_add_string ("Starting sysinfo..");
		}
	  else if (str_cmp (console_manager_current_line, "doom") == 0)
		{
		  console_manager.next_program = ISENOS_PROGRAMS_DOOM;
		  text_output_manager_add_string ("Starting doom..");
		}
	  else if (str_cmp (console_manager_current_line, "donut") == 0)
		{
		  console_manager.next_program = ISENOS_PROGRAMS_DONUT;
		  text_output_manager_add_string ("Starting donut..");
		}
	  else
		{
		  console_manager.next_program = ISENOS_PROGRAMS_NONE;
		  text_output_manager_add_string ("ERROR: Unknown program !");
		}

	  console_manager_reset_input();
	  text_output_manager_add_string ("ring0@isen-os: ");
	}
  else if (scan_code == 0x0E)
	{
	  if (console_manager_current_line_length == 0)
		{
		  return;
		}
	  text_output_manager_remove_char (1);
	  console_manager_current_line[console_manager_current_line_length--] = 0;
	}
}
isenos_programs_e console_manager_get_next_program ()
{
  if (console_manager.next_program != ISENOS_PROGRAMS_NONE)
	{
	  isenos_programs_e next_program = console_manager.next_program;
	  console_manager.next_program = ISENOS_PROGRAMS_NONE;
	  return next_program;
	}
  return ISENOS_PROGRAMS_NONE;
}
void console_manager_reset_input()
{
  console_manager_current_line_length = 0;
  memset (console_manager_current_line, sizeof (console_manager_current_line) / 8, 0);
}


void console_manager_init ()
{
  console_manager_reset_input();
}
