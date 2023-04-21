#ifndef __UTILS__
#define __UTILS__

#include "inttypes.h"

/**
 * @brief function to get the 16 lowest bits of a word
 * 
 * @param[in] addr the value that should be retrieved
 * 
 * @return u16 the lowest 16 bits
 */
#define low16(addr) (u16)((addr) & 0xFFFF)

/**
 * @brief function to get the 16 highest bits of a word
 * 
 * @param[in] addr the value that should be retrieved
 * 
 * @return u16 the highest 16 bits
 */
#define high16(addr) (u16)(((addr) >> 16) & 0xFFFF)

/**
 * @brief function converting an int to a string
 * 
 * @param[in] n the value that should be stringified
 * @param[out] str the char* that will contain the result
 * 
 * @return void
 */
void iToA(int32_t, char*);

/**
 * @brief computes the value of the power of a number
 * 
 * @param[in] base the number that will be elevated
 * @param[in] exp the exponent. It must be integer.
 * 
 * @return f64 the result of the power
 */
double powk(double, int64_t);

#endif