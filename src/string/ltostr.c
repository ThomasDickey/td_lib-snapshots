#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/string/RCS/ltostr.c,v 8.0 1988/05/12 09:09:33 ste_cm Rel $";
#endif	lint

/*
 * Title:	ltostr.c (long-to-string)
 * Author:	T.E.Dickey
 * Created:	12 May 1988
 * $Log: ltostr.c,v $
 * Revision 8.0  1988/05/12 09:09:33  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  88/05/12  09:09:33  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/05/12  09:09:33  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/05/12  09:09:33  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/05/12  09:09:33  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/05/12  09:09:33  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/05/12  09:09:33  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  88/05/12  09:09:33  dickey
 *		sccs2rcs keywords
 *		
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

char *
ltostr(src, value, base)
char	*src;
long	value;
int	base;
{
static	char	digits[] = "0123456789abcdef";
static	long	mask16, mask8, mask2;

	if (mask2 == 0) {
	register long	mask = 1;
	register int	bits = 1, j;
		while (mask <<= 1)	bits++;
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
		*src++ = digits[value];
	else {
	int	remain;
		if (base == 10) {
		long	next	= ((value >> 1) & mask2) / 5;
			remain	= value - (next * 10);
			value	= next;
		} else { /* right-shift keeps sign, so we must make masks */
			remain = value & (base-1);
			if (base == 16) {
				value >>= 4;
				value &= mask16;
			} else {
				value >>= 3;
				value &= mask8;
			}
		}
		src = ltostr(src, value, base);
		*src++ = digits[remain];
	}

	*src = '\0';
	return (src);
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
extern	long	strtol();
int	j;

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
			(void)ltostr(bfr, value,   0); printf("%s, ", bfr);
			(void)ltostr(bfr, value,  -8); printf("%s, ", bfr);
			(void)ltostr(bfr, value, -16); printf("%s\n", bfr);
		}
	}
	exit(0);
	/*NOTREACHED*/
}
#endif	TEST
