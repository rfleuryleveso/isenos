//
// Created by rfleuryleveso on 21/04/23.
//

#include "text_output_manager.h"
#include "../print.h"
volatile char graphics_lines[TOM_ROWS][TOM_COLUMNS];
uint8_t tom_current_line = 0;
uint8_t tom_current_line_length = 0;

void text_output_manager_clear ()
{
  memset (graphics_lines,  0, TOM_COLUMNS * TOM_ROWS);
  tom_current_line = 0;
  tom_current_line_length = 0;
}

// This will copy the line to the above line
void text_output_manager_move_line (uint8_t src, uint8_t dst)
{
  if (src == dst)
	{
	  return;
	}
  if (src > TOM_ROWS - 1 || dst > TOM_ROWS - 1)
	{
	  return;
	}
  memcpy (graphics_lines[dst], graphics_lines[src], TOM_COLUMNS);
}

// This will copy the line to the above line
void text_output_manager_move_all_line_up (uint8_t count)
{

  for (uint8_t index = 0; index < count; index++)
	{
	  for (uint8_t line = 1; line <= TOM_ROWS; line++)
		{
		  memset (graphics_lines[line-1],  0, TOM_COLUMNS);
		  text_output_manager_move_line (line, line - 1);
		}
	  tom_current_line--;
	}
}

void text_output_manager_add (char ascii_code)
{
  if (tom_current_line_length == TOM_COLUMNS - 1)
	{
	  text_output_manager_new_line ();
	}

  if (ascii_code == '\n')
	{
	  text_output_manager_new_line ();
	}

  graphics_lines[tom_current_line][tom_current_line_length++] = ascii_code;
}

void text_output_manager_add_string (char *str)
{
  // LMAO THIS IS ULTRA MEGA UNSAFE
  // this will lead to issues.
  while (*str != 0)
	{
	  text_output_manager_add (*str);
	  str++;
	}
}
void text_output_manager_new_line ()
{
  // If we are at the last line
  if (tom_current_line == TOM_ROWS - 1)
	{
	  text_output_manager_move_all_line_up (1);
	}
  else
	{
	  tom_current_line++;
	}
  tom_current_line_length = 0;
}
void text_output_manager_remove_char (uint64_t count)
{
  uint64_t current_character_index = tom_current_line_length - 1;
  if (current_character_index - count < 0)
	{
	  count = current_character_index;
	}

  // If we are at the last line
  for (uint64_t index = count; index > 0; index--)
	{
	  graphics_lines[tom_current_line][tom_current_line_length - 1] = 0;
	  tom_current_line_length--;
	}
}
