//
// Created by rfleuryleveso on 19/04/23.
//

#ifndef _GRAPHICS_MANAGER_H_
#define _GRAPHICS_MANAGER_H_

#include "../isenos.h"


typedef struct graphics_manager_runtime_config_t {
	uint32_t text_color;
};

extern struct graphics_manager_runtime_config_t gm_runtime_config;

extern IBL_FRAMEBUFFER_INFO framebuffer_info;

void gm_init (IBL_ISENOS_DATA *ibl_isenos_data);
void gm_clear();
void gm_render();
void gm_draw_pixel32bpp (int x, int y, uint32_t pixel);

#endif //_GRAPHICS_MANAGER_H_
