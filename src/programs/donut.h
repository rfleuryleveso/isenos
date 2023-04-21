//
// Created by rfleuryleveso on 21/04/23.
//

#ifndef _DONUT_H_
#define _DONUT_H_

#include <stdbool.h>
#include "../graphics/text_output_manager.h"
#include "../common/math.h"
#include "../pit/pit.h"

extern const float theta_spacing;
extern const float phi_spacing;

extern const float R1;
extern const float R2;
extern const float K2;
// Calculate K1 based on screen size: the maximum x-distance occurs
// roughly at the edge of the torus, which is at x=R1+R2, z=0.  we
// want that to be displaced 3/8ths of the width of the screen, which
// is 3/4th of the way from the center to the side of the screen.
// screen_width*3/8 = K1*(R1+R2)/(K2+0)
// screen_width*K2*3/(8*(R1+R2)) = K1
extern const float K1;

void donut ();

#endif //_DONUT_H_
