//
// Created by rfleuryleveso on 18/04/23.
//

#ifndef _MATH_H_
#define _MATH_H_

#define M_PI (3.1415927f)
#define M_PI_2 (M_PI/2.0f)
#define M_PI_M_2 (M_PI*2.0f)

union float_int {
	float f;
	int i;
};

float ceil (float x);

double cos(double x);


double sin(double x);

#endif //_MATH_H_
