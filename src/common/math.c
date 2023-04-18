//
// Created by rfleuryleveso on 18/04/23.
//

#include "math.h"

float ceil (float x)
{
  union float_int val;
  val.f = x;

// Extract sign, exponent and mantissa
// Bias is removed from exponent
  int sign = val.i >> 31;
  int exponent = ((val.i & 0x7fffffff) >> 23) - 127;
  int mantissa = val.i & 0x7fffff;

// Is the exponent less than zero?
  if (exponent < 0)
	{
// In this case, x is in the open interval (-1, 1)
	  if (x <= 0.0f)
		return 0.0f;
	  else
		return 1.0f;
	}
  else
	{
// Construct a bit mask that will mask off the
// fractional part of the mantissa
	  int mask = 0x7fffff >> exponent;

// Is x already an integer (i.e. are all the
// fractional bits zero?)
	  if ((mantissa & mask) == 0)
		return x;
	  else
		{
// If x is positive, we need to add 1 to it
// before clearing the fractional bits
		  if (!sign)
			{
			  mantissa += 1 << (23 - exponent);

// Did the mantissa overflow?
			  if (mantissa & 0x800000)
				{
// The mantissa can only overflow if all the
// integer bits were previously 1 -- so we can
// just clear out the mantissa and increment
// the exponent
				  mantissa = 0;
				  exponent++;
				}
			}

// Clear the fractional bits
		  mantissa &= ~mask;
		}
	}

// Put sign, exponent and mantissa together again
  val.i = (sign << 31) | ((exponent + 127) << 23) | mantissa;

  return val.f;
}