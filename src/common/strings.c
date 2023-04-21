/**
 * @file char*s.c
 * @author Théodore MARTIN
 * @brief function library to imitate char*s.c from the stdlib
 * @version 0.1
 * @date 2023-03-22
 */
#include "./strings.h"
#include "utils.h"
#include "os-types.h"

uint32_t str_len(char *str)
{
  uint32_t result = 0;
  while(str[result] != '\0') {
	  result++;
	}
  return result;
}

int8_t str_cmp(const char *str1, const char *str2)
{
  while (*str1)
	{
	  // if characters differ, or end of the second string is reached
	  if (*str1 != *str2) {
		  break;
		}

	  // move to the next pair of characters
	  str1++;
	  str2++;
	}

  // return the ASCII difference after converting `char*` to `unsigned char*`
  return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

void strReverse(char *str)
{
  char temp;
  uint32_t size = str_len(str);
  for (uint32_t i = 0; i < size/2; ++i) {
	  temp = str[i];
	  str[i] = str[size - i - 1];
	  str[size - i - 1] = temp;
	}
}

uint32_t str_cpy(char* in, char* out)
{
  if (in == NULL || out == NULL)
	return -1;
  while (*in != '\0') {
	  *out++ = *in++;
	}
  return 0;
}

void append(char *str, char n)
{
  uint32_t appendPoint = str_len(str);
  str[appendPoint] = n;
  str[appendPoint + 1] = '\0';
}

uint32_t wordCount(char *str, char del)
{
  if (*str == '\0')
	return 0;

  uint32_t result = 1;
  uint8_t detected = 0;
  while(*str++ != '\0') {
	  if (*str == '\"')
		detected = !detected;
	  if (*str == del && *(str+1) != del && !detected)
		result++;
	}
  return result;
}

uint32_t biggestWord(char *str, char del)
{
  uint32_t biggest = 0;
  uint32_t temp = 0;
  while (*str++ != '\0') {
	  if (*str == del) {
		  if (temp > biggest)
			biggest = temp;
		  temp = 0;
		} else {
		  temp++;
		}
	}
  return biggest;
}



static void strLenBiggestWordAndWordNb(char *str, char del, uint32_t *size, uint32_t *biggest, uint32_t *wordNb)
{
  uint32_t temp = 0;
  uint8_t wordPreviouslyFound = 0;
  *biggest = *size = *wordNb = 0;
  do {
	  if (*str == del || *str == '\0') {
		  if (temp > *biggest)
			*biggest = temp;
		  if (wordPreviouslyFound)
			*(wordNb) += 1;
		  wordPreviouslyFound = 0;
		  temp = 0;
		} else {
		  wordPreviouslyFound = 1;
		  temp++;
		}
	  *(size) += 1;
	} while (*str++ != '\0');
}


static int32_t charIndex(char *str, char searched)
{
  int32_t res = 0;
  while(str[res] != searched && str[res] != '\0') {
	  res++;
	}
  return str[res] == '\0' ? -1 : res;
}

double strToL(char *str)
{
  if (str == NULL || str[0] == '\0')
	return 0;
  double res = 0;
  uint8_t neg = str[0] == '-';
  uint32_t index = neg ? 1 : 0;
  int32_t wholePart = charIndex(str, '.');
  if (wholePart == -1) wholePart = str_len(str);
  while (str[index] != '\0') {
	  if (str[index] == '.') {
		  wholePart++;
		} else {

		  res += (str[index] - 48) * powk(10, wholePart - 1 - index);
		}
	  index++;
	}
  res = neg ? res * -1 : res;
  return res;
}

void hexToAscii(int n, char *str) {
  append(str, '0');
  append(str, 'x');
  char zeros = 0;
  uint32_t tmp;
  int i;
  for (i = 28; i > 0; i -= 4) {
	  tmp = (n >> i) & 0xF;
	  if (tmp == 0 && zeros == 0) continue;
	  zeros = 1;
	  if (tmp > 0xA) append(str, tmp - 0xA + 'a');
	  else append(str, tmp + '0');
	}

  tmp = n & 0xF;
  if (tmp >= 0xA) append(str, tmp - 0xA + 'a');
  else append(str, tmp + '0');
}