//
// Created by rfleuryleveso on 17/04/23.
//

#include "print.h"
#include "graphics/text_output_manager.h"

boolean_t _doprnt_truncates = FALSE;

int print_initialize_com1 ()
{
  outb (COM1_ADDR + 1, 0x00);    // Disable all interrupts
  outb (COM1_ADDR + 3, 0x80);    // Enable DLAB (set baud rate divisor)
  outb (COM1_ADDR + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
  outb (COM1_ADDR + 1, 0x00);    //                  (hi byte)
  outb (COM1_ADDR + 3, 0x03);    // 8 bits, no parity, one stop bit
  outb (COM1_ADDR + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
  outb (COM1_ADDR + 4, 0x0B);    // IRQs enabled, RTS/DSR set
  outb (COM1_ADDR + 4, 0x1E);    // Set in loopback mode, test the serial chip
  outb (COM1_ADDR + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

  // Check if serial is faulty (i.e: not same byte as sent)
  if (inb (COM1_ADDR + 0) != 0xAE)
	{
	  return 1;
	}

  // If serial is not faulty set it in normal operation mode
  // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
  outb (COM1_ADDR + 4, 0x0F);
  return 0;
}
int serial_received ()
{
  return inb (COM1_ADDR + 5) & 1;
}

char read_serial ()
{
  while (serial_received () == 0);

  return inb (COM1_ADDR);
}

int is_transmit_empty ()
{
  return inb (COM1_ADDR + 5) & 0x20;
}

void write_serial (char a)
{
  while (is_transmit_empty () == 0);

  outb (COM1_ADDR, a);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
void
printnum (
	register unsigned int u,        /* number to print */
	register int base,
	void            (*putc) (char))
{
  char buf[MAXBUF];    /* build number here */
  register char *p = &buf[MAXBUF - 1];
  static char digs[] = "0123456789abcdef";

  do
	{
	  *p-- = digs[u % base];
	  u /= base;
	}
  while (u != 0);

  while (++p != &buf[MAXBUF])
	(*putc) (*p);

}

void
_doprnt (
	register const char *fmt,
	va_list *argp,
	/* character output routine */
	void            (*putc) (char),
	int radix)        /* default radix - for '%r' */
{
  int length;
  int prec;
  boolean_t ladjust;
  char padc;
  long n;
  unsigned long u;
  int plus_sign;
  int sign_char;
  boolean_t altfmt, truncate;
  int base;
  register char c;
  int capitals;

  while ((c = *fmt) != '\0')
	{
	  if (c != '%')
		{
		  (*putc) (c);
		  fmt++;
		  continue;
		}

	  fmt++;

	  length = 0;
	  prec = -1;
	  ladjust = FALSE;
	  padc = ' ';
	  plus_sign = 0;
	  sign_char = 0;
	  altfmt = FALSE;

	  while (TRUE)
		{
		  c = *fmt;
		  if (c == '#')
			{
			  altfmt = TRUE;
			}
		  else if (c == '-')
			{
			  ladjust = TRUE;
			}
		  else if (c == '+')
			{
			  plus_sign = '+';
			}
		  else if (c == ' ')
			{
			  if (plus_sign == 0)
				plus_sign = ' ';
			}
		  else
			break;
		  fmt++;
		}

	  if (c == '0')
		{
		  padc = '0';
		  c = *++fmt;
		}

	  if (isdigit(c))
		{
		  while (isdigit(c))
			{
			  length = 10 * length + Ctod(c);
			  c = *++fmt;
			}
		}
	  else if (c == '*')
		{
		  length = va_arg(*argp, int);
		  c = *++fmt;
		  if (length < 0)
			{
			  ladjust = !ladjust;
			  length = -length;
			}
		}

	  if (c == '.')
		{
		  c = *++fmt;
		  if (isdigit(c))
			{
			  prec = 0;
			  while (isdigit(c))
				{
				  prec = 10 * prec + Ctod(c);
				  c = *++fmt;
				}
			}
		  else if (c == '*')
			{
			  prec = va_arg(*argp, int);
			  c = *++fmt;
			}
		}

	  if (c == 'l')
		c = *++fmt;    /* need it if sizeof(int) < sizeof(long) */

	  truncate = FALSE;
	  capitals = 0;        /* Assume lower case printing */

	  switch (c)
		{
	  case 'b':
	  case 'B':
		{
		  register char *p;
		  boolean_t any;
		  register int i;

		  u = va_arg(*argp, unsigned long);
		  p = va_arg(*argp, char *);
		  base = *p++;
		  printnum (u, base, putc);

		  if (u == 0)
			break;

		  any = FALSE;
		  while ((i = *p++) != '\0')
			{
			  if (*fmt == 'B')
				i = 33 - i;
			  if (*p <= 32)
				{
				  /*
				   * Bit field
				   */
				  register int j;
				  if (any)
					(*putc) (',');
				  else
					{
					  (*putc) ('<');
					  any = TRUE;
					}
				  j = *p++;
				  if (*fmt == 'B')
					j = 32 - j;
				  for (; (c = *p) > 32; p++)
					(*putc) (c);
				  printnum ((unsigned)((u >> (j - 1)) & ((2 << (i - j)) - 1)),
							base, putc);
				}
			  else if (u & (1 << (i - 1)))
				{
				  if (any)
					(*putc) (',');
				  else
					{
					  (*putc) ('<');
					  any = TRUE;
					}
				  for (; (c = *p) > 32; p++)
					(*putc) (c);
				}
			  else
				{
				  for (; *p > 32; p++)
					continue;
				}
			}
		  if (any)
			(*putc) ('>');
		  break;
		}

	  case 'c':
		c = va_arg(*argp, int);
		  (*putc) (c);
		  break;

	  case 's':
		{
		  register char *p;
		  register char *p2;

		  if (prec == -1)
			prec = 0x7fffffff;    /* MAXINT */

		  p = va_arg(*argp, char *);

		  if (p == (char *)0)
			p = "";

		  if (length > 0 && !ladjust)
			{
			  n = 0;
			  p2 = p;

			  for (; *p != '\0' && n < prec; p++)
				n++;

			  p = p2;

			  while (n < length)
				{
				  (*putc) (' ');
				  n++;
				}
			}

		  n = 0;

		  while (*p != '\0')
			{
			  if (++n > prec || (length > 0 && n > length))
				break;

			  (*putc) (*p++);
			}

		  if (n < length && ladjust)
			{
			  while (n < length)
				{
				  (*putc) (' ');
				  n++;
				}
			}

		  break;
		}

	  case 'o':
		truncate = _doprnt_truncates;
	  case 'O':
		base = 8;
		  goto print_unsigned;

	  case 'd':
		truncate = _doprnt_truncates;
	  case 'D':
		base = 10;
		  goto print_signed;

	  case 'u':
		truncate = _doprnt_truncates;
	  case 'U':
		base = 10;
		  goto print_unsigned;

	  case 'p':
		altfmt = TRUE;
	  case 'x':
		truncate = _doprnt_truncates;
		  base = 16;
		  goto print_unsigned;

	  case 'X':
		base = 16;
		  capitals = 16;    /* Print in upper case */
		  goto print_unsigned;

	  case 'z':
		truncate = _doprnt_truncates;
		  base = 16;
		  goto print_signed;

	  case 'Z':
		base = 16;
		  capitals = 16;    /* Print in upper case */
		  goto print_signed;

	  case 'r':
		truncate = _doprnt_truncates;
	  case 'R':
		base = radix;
		  goto print_signed;

	  case 'n':
		truncate = _doprnt_truncates;
	  case 'N':
		base = radix;
		  goto print_unsigned;

	  print_signed:
		n = va_arg(*argp, long);
		  if (n >= 0)
			{
			  u = n;
			  sign_char = plus_sign;
			}
		  else
			{
			  u = -n;
			  sign_char = '-';
			}
		  goto print_num;

	  print_unsigned:
		u = va_arg(*argp, unsigned long);
		  goto print_num;

	  print_num:
	  {
		char buf[MAXBUF];    /* build number here */
		register char *p = &buf[MAXBUF - 1];
		static char digits[] = "0123456789abcdef0123456789ABCDEF";
		char *prefix = 0;

		if (truncate) u = (long)((int)(u));

		if (u != 0 && altfmt)
		  {
			if (base == 8)
			  prefix = "0";
			else if (base == 16)
			  prefix = "0x";
		  }

		do
		  {
			/* Print in the correct case */
			*p-- = digits[(u % base) + capitals];
			u /= base;
		  }
		while (u != 0);

		length -= (&buf[MAXBUF - 1] - p);
		if (sign_char)
		  length--;
		if (prefix)
		  length -= str_len ((const char *)prefix);

		if (padc == ' ' && !ladjust)
		  {
			/* blank padding goes before prefix */
			while (--length >= 0)
			  (*putc) (' ');
		  }
		if (sign_char)
		  (*putc) (sign_char);
		if (prefix)
		  while (*prefix)
			(*putc) (*prefix++);
		if (padc == '0')
		  {
			/* zero padding goes after sign and prefix */
			while (--length >= 0)
			  (*putc) ('0');
		  }
		while (++p != &buf[MAXBUF])
		  (*putc) (*p);

		if (ladjust)
		  {
			while (--length >= 0)
			  (*putc) (' ');
		  }
		break;
	  }

	  case '\0':
		fmt--;
		  break;

	  default:
		(*putc) (c);
		}
	  fmt++;
	}
}

void
printf (const char *fmt, ...)
{
  va_list listp;

  va_start(listp, fmt);
  _doprnt (fmt, &listp, write_serial, 16);
  va_end(listp);
}

void
printf_ui (const char *fmt, ...)
{
  va_list listp;

  va_start(listp, fmt);
  _doprnt (fmt, &listp, text_output_manager_add, 16);
  va_end(listp);
}

#pragma GCC diagnostic pop