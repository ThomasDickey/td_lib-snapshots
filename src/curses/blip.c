#ifndef	lint
static	char	sccs_id[] = "@(#)blip.c	1.1 88/08/11 09:34:40";
#endif	lint

/*
 * Title:	blip.c (write a single character to screen)
 * Author:	T.E.Dickey
 * Created:	11 Aug 1988, from 'beep()'
 * Modified:
 */

#include	<stdio.h>

blip(c)
{
	if (putc(c, stderr) != EOF)
		(void)fflush(stderr);
}
