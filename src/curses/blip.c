#ifndef	lint
static	char	Id[] = "$Id: blip.c,v 12.0 1992/11/18 08:57:22 ste_cm Rel $";
#endif

/*
 * Title:	blip.c (write a single character to screen)
 * Author:	T.E.Dickey
 * Created:	11 Aug 1988, from 'beep()'
 * Modified:
 *		18 Oct 1991, if stderr is not a terminal, open directly to
 *			     /dev/tty.
 *		04 Oct 1991, conversion to ANSI
 *		20 Jun 1991, changed return-type to 'void'
 */

#define	CUR_PTYPES
#include	<ptypes.h>

void blip(
_AR1(int,	c))
_DCL(int,	c)
{
	static	FILE	*fp;
	if (fp == 0) {
		if (stderr != 0 && isatty(fileno(stderr)))
			fp = stderr;
#ifdef	unix
		else
			fp = fopen("/dev/tty", "w");
#endif
		if (fp == 0)
			return;		/* it's not that important */
	}
#ifdef	putc
	c = putc((unsigned char)c, fp);
#else
	c = putc(c, fp);
#endif
	if (c != EOF)
		FFLUSH(fp);
}
