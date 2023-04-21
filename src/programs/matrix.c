//
// Created by rfleuryleveso on 21/04/23.
//

#include <stdbool.h>
#include "matrix.h"
#include "../graphics/text_output_manager.h"
#include "../scheduler/scheduler.h"
#include "../pit/pit.h"
#include "../graphics/graphics_manager.h"

uint64_t matrix_random_seed = 6454645;

int fake_random ()
{
  int a = 1103515245;
  int c = 12345;
  uint64_t m = 1LL << 31;
  matrix_random_seed = (a * matrix_random_seed + c) % m;
  return matrix_random_seed;
}
void matrix ()
{
  matrix_random_seed = pit_counter_1000hz;
// Define the set of characters to use
  char chars[] = "01azertyuiopqsdfghjklmwxcvbn";

  // Define the initial state of the scrolling characters
  for (int i = 0; i < TOM_ROWS; i++)
	{
	  for (int j = 0; j < TOM_COLUMNS; j++)
		{
		  graphics_lines[i][j] = chars[fake_random () % sizeof (chars) / sizeof (char)];
		}
	}

  // Start the scrolling effect
  while (true)
	{
	  // Scroll the matrix by one row
	  for (int i = TOM_ROWS - 1; i > 0; i--)
		{
		  for (int j = 0; j < TOM_COLUMNS; j++)
			{
			  graphics_lines[i][j] = graphics_lines[i - 1][j];
			}
		}

	  // Generate a new row of random characters
	  for (int j = 0; j < TOM_COLUMNS; j++)
		{
		  graphics_lines[0][j] = chars[fake_random () % sizeof (chars) / sizeof (char)];
		}
	  gm_render ();
	  wait_ms (100);
	  gm_clear ();
	  if (scheduler_program_should_stop ())
		{
		  text_output_manager_clear ();
		  text_output_manager_add_string ("Welcome to ISENOS v1.1\0");
		  text_output_manager_new_line ();
		  text_output_manager_add_string ("ring0@isen-os: \0");
		  return;
		}
	}
}
