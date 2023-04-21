//
// Created by rfleuryleveso on 19/04/23.
//

#include "../memory/memory_tools.h"
#include "graphics_manager.h"
#include "fonts/font8x8_basic.h"
#include "text_output_manager.h"

IBL_FRAMEBUFFER_INFO framebuffer_info = { 0 };
struct graphics_manager_runtime_config_t gm_runtime_config;

#pragma GCC push_options
#pragma GCC optimize ("O0")
void gm_draw_pixel32bpp (int x, int y, uint32_t pixel)
{
  *((uint32_t *)(framebuffer_info.FrameBufferBase + 4 * framebuffer_info.PixelsPerScanLine * y + 4 * x)) = pixel;
}
void fill_screen (uint64_t start_x, uint64_t end_x, uint64_t start_y, uint64_t end_y, uint32_t color)
{

  for (int x = start_x; x < end_x; x++)
	{
	  for (int y = start_y; y < end_y; y++)
		{
		  gm_draw_pixel32bpp (x, y, color);
		}
	}
}
void splash_screen (uint64_t start_x, uint64_t end_x, uint64_t start_y, uint64_t end_y)
{

  for (int x = start_x; x < end_x; x++)
	{
	  for (int y = start_y; y < end_y; y++)
		{

		  gm_draw_pixel32bpp (x, y, (start_y + start_y) % 0xFFFFFF);
		}
	}
}
void gm_init (IBL_ISENOS_DATA *ibl_isenos_data)
{
  gm_runtime_config.text_color = 0xFFFFFF;
  gm_runtime_config.background_color = 0x000000;
  memcpy (&framebuffer_info, ibl_isenos_data->FrameBufferInfo, sizeof (IBL_FRAMEBUFFER_INFO));
  splash_screen (0, framebuffer_info.HorizontalResolution, 0, framebuffer_info.VerticalResolution);
}
void gm_clear ()
{
  fill_screen (0, framebuffer_info.HorizontalResolution, 0, framebuffer_info.VerticalResolution, gm_runtime_config.background_color);
}
void gm_render ()
{
  for (int x = 0; x < TOM_COLUMNS; x++)
	{

	  for (int y = 0; y < TOM_ROWS; y++)
		{
		  char _char = (char)graphics_lines[y][x];
		  if (_char >= 32 && _char <= 126)
			{
			  for (int font_x = 0; font_x < 8; font_x++)
				{
				  for (int font_y = 0; font_y < 8; font_y++)
					{
					  int relative_x = x * 8 + x * 1 + font_x;
					  int relative_y = y * 8 + y * 1 + font_y;

					  char *font = font8x8_basic[_char];
					  char font_line = font[font_y];
					  uint8_t visible = font_line & (1 << font_x);
					  gm_draw_pixel32bpp (relative_x, relative_y, visible ? gm_runtime_config.text_color : gm_runtime_config.background_color);
					}
				}
			}
		}
	}

}
#pragma GCC pop_options