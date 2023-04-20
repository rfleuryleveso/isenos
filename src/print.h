//
// Created by rfleuryleveso on 17/04/23.
//

#ifndef ISENOS_PRELOADER_PRINT_H
#define ISENOS_PRELOADER_PRINT_H

#define COM1_ADDR 0x3F8

#include <stdarg.h>
#include "isenos.h"
#include "stdint.h"
#include "common/ports.h"
#include "common/string.h"

#define isdigit(d) ((d) >= '0' && (d) <= '9')
#define Ctod(c) ((c) - '0')

#define MAXBUF (sizeof(long int) * 8)         /* enough for binary */

void
printnum (
	register unsigned int u,        /* number to print */
	register int base,
	void            (*putc) (char));

int print_initialize_com1 ();

void _doprnt (
	register const char *fmt,
	va_list *argp,
	/* character output routine */
	void            (*putc) (char),
	int radix);

void
printf (const char *fmt, ...);

#endif //ISENOS_PRELOADER_PRINT_H
