//
// Created by rfleuryleveso on 21/04/23.
//

#include "colorscroll.h"
#include "../pit/pit.h"
#include "../graphics/graphics_manager.h"

_Noreturn void color_scroll ()
{

  //
  while (1 == 1)
	{
	  for (int x = 0; x < framebuffer_info.HorizontalResolution; x++)
		{
		  for (int y = 0; y < framebuffer_info.VerticalResolution; y++)
			{



			  float xPercent = (float)((x + pit_counter_1000hz / 100) % framebuffer_info.HorizontalResolution)
							   / framebuffer_info.HorizontalResolution;
			  float yPercent = (float)((y + pit_counter_1000hz / 100) % framebuffer_info.VerticalResolution)
							   / framebuffer_info.VerticalResolution;

			  uint32_t r = yPercent * 0xFF;
			  uint32_t g = yPercent * 0xFF;
			  uint32_t b = yPercent *  0xFF;
			  gm_draw_pixel32bpp (x, y, (r << 16) + (g << 8) + b);
			}
		}
	}
}
