#ifndef	lint
static	char	Id[] = "$Id: blip.c,v 9.1 1991/06/20 08:42:05 dickey Exp $";
#endif

/*
 * Title:	blip.c (write a single character to screen)
 * Author:	T.E.Dickey
 * Created:	11 Aug 1988, from 'beep()'
 * Modified:
 *		20 Jun 1991, changed return-type to 'void'
 */

#include	"ptypes.h"

void blip(c)
{
#ifdef	putc
	c = putc((unsigned char)c, stderr);
#else
	c = putc(c, stderr);
#endif
	if (c != EOF)
		(void)fflush(stderr);
}
