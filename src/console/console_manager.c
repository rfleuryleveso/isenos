//
// Created by rfleuryleveso on 21/04/23.
//

#include "console_manager.h"
#include "../print.h"
#include "../graphics/text_output_manager.h"

volatile char console_manager_current_line[256];
volatile uint8_t console_manager_current_line_length;

void console_manager_keyboard_callback (uint8_t scan_code)
{
  printf ("Received keyboard scan_code = %d \n", scan_code);
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
	  memset (console_manager_current_line, sizeof (console_manager_current_line) / sizeof (char), 0);
	  text_output_manager_add_string ("ring0@isen-os: ");
	}
  else if (scan_code == 0x0E)
	{
	  if(console_manager_current_line_length == 0) {
		return;
	  }
	  text_output_manager_remove_char (1);
	  console_manager_current_line[console_manager_current_line_length--] = 0;
	}
}
