/*
 * Title:	ltostr.c (long-to-string)
 * Author:	T.E.Dickey
 * Created:	12 May 1988
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		
 * Function:	This is the functional inverse of 'strtol()'.  It
 *		converts a long-value to string without using sprintf
 *		so that it is relatively self-contained.  The code will
 *		convert even very large numbers.
 *
 * Arguments:	src	points to the buffer in which to store the string
 *		value	is the number to convert
 *		base	is the conversion base (8,10,16).  If negative, and
 *			the value is nonzero, expand a base-prefix a la
 *			'sprintf' for '%#x' or '%#o'.
 *
 * Returns:	A pointer to the null ending the converted string.
 */

#include "ptypes.h"

MODULE_ID("$Id: ltostr.c,v 12.5 1995/02/11 19:21:05 tom Exp $")

char *
l2str(
_ARX(char *,	src)
_ARX(long,	value)
_AR1(int,	base)
	)
_DCL(char *,	src)
_DCL(long,	value)
_DCL(int,	base)
{
static	char	digits[] = "0123456789abcdef";
static	long	mask16, mask8, mask2;

	if (mask2 == 0) {
		register long	mask = 1;
		register int	bits = 1, j;

		while ((mask <<= 1) != 0)
			bits++;
		for (j = 1; j < bits; j++) {
			mask2 <<= 1;
			mask2 |=  1;
		}
		mask8  = mask2 >> 2;
		mask16 = mask8 >> 1;
	}

	if (base < 0) {
		base = -base;
		if (value) {
			*src++ = '0';
			if (base == 16)
				*src++ = 'x';
		}
	}
	if (base != 8 && base != 16)	base = 10;

	/* check for negative numbers if decimal */
	if (base == 10 && value < 0) {
		value = -value;
		*src++ = '-';
	}

	if (value >= 0 && value < base)
		*src++ = digits[(int)value];
	else {
		int	remain;
		if (base == 10) {
			long	next	= ((value >> 1) & mask2) / 5;
			remain	= (int)(value - (next * 10));
			value	= next;
		} else { /* right-shift keeps sign, so we must make masks */
			remain = (int)(value & (base-1));
			if (base == 16) {
				value >>= 4;
				value &= mask16;
			} else {
				value >>= 3;
				value &= mask8;
			}
		}
		src = l2str(src, value, base);
		*src++ = digits[remain];
	}

	*src = EOS;
	return (src);
}

#ifdef	TEST
_MAIN
{
	register int	j;

	for (j = 1; j < argc; j++) {
	char	*d, *s	= argv[j];
	long	value	= strtol(s, &d, 0);
		if (*d) {
			printf("?? %s\n", s);
			printf("---");
			while (d-- > s)
				printf("-");
			printf("^\n");
		} else {
		char	bfr[132];
			printf("** %ld, %#lo, %#lx\n", value, value, value);
			printf("=> ");
			(void)l2str(bfr, value,   0); printf("%s, ", bfr);
			(void)l2str(bfr, value,  -8); printf("%s, ", bfr);
			(void)l2str(bfr, value, -16); printf("%s\n", bfr);
		}
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
