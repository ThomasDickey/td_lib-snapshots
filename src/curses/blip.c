/*
 * Title:	blip.c (write a single character to screen)
 * Author:	T.E.Dickey
 * Created:	11 Aug 1988, from 'beep()'
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		18 Oct 1991, if stderr is not a terminal, open directly to
 *			     /dev/tty.
 *		04 Oct 1991, conversion to ANSI
 *		20 Jun 1991, changed return-type to 'void'
 */

#include	"ptypes.h"

MODULE_ID("$Id: blip.c,v 12.5 2001/05/15 01:00:14 tom Exp $")

void blip(
_AR1(int,	c))
_DCL(int,	c)
{
	static	FILE	*fp;
	if (fp == 0) {
		if (stderr != 0 && isatty(fileno(stderr)))
			fp = stderr;
#ifdef	SYS_UNIX
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
