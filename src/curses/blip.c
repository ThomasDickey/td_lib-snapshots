#ifndef	lint
static	char	Id[] = "$Id: blip.c,v 9.3 1991/10/04 12:33:22 dickey Exp $";
#endif

/*
 * Title:	blip.c (write a single character to screen)
 * Author:	T.E.Dickey
 * Created:	11 Aug 1988, from 'beep()'
 * Modified:
 *		04 Oct 1991, conversion to ANSI
 *		20 Jun 1991, changed return-type to 'void'
 */

#include	"ptypes.h"

void blip(
_AR1(int,	c))
_DCL(int,	c)
{
#ifdef	putc
	c = putc((unsigned char)c, stderr);
#else
	c = putc(c, stderr);
#endif
	if (c != EOF)
		FFLUSH(stderr);
}
