//
// Created by rfleuryleveso on 19/04/23.
//

#include "../memory/memory_tools.h"
#include "graphics_manager.h"

IBL_FRAMEBUFFER_INFO framebuffer_info = {0};

static inline void gm_draw_pixel32bpp(int x, int y, uint32_t pixel)
{
  *((uint32_t*)(framebuffer_info.FrameBufferBase + 4 * framebuffer_info.PixelsPerScanLine * y + 4 * x)) = pixel;
}

void gm_init (IBL_ISENOS_DATA *ibl_isenos_data)
{
	memcpy(&framebuffer_info, ibl_isenos_data->FrameBufferInfo, sizeof(IBL_FRAMEBUFFER_INFO));

	for(int x = 0; x < framebuffer_info.HorizontalResolution; x++) {
		for(int y = 0; y < framebuffer_info.VerticalResolution; y++) {
			gm_draw_pixel32bpp(x, y, 0x00FFFFFF);
		  }
	}

}
