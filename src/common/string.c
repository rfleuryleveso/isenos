//
// Created by rfleuryleveso on 17/04/23.
//

#include "string.h"

unsigned int strlen (const char *s)
{
  unsigned int count = 0;
  while (*s != '\0')
	{
	  count++;
	  s++;
	}
  return count;
}