#ifndef	lint
static	char	sccs_id[] = "@(#)beep.c	1.1 88/03/24 11:37:48";
#endif	lint

/*
 * Title:	beep.c (sound audible alarm)
 * Author:	T.E.Dickey
 * Created:	24 Mar (from 'ded.c')
 * Modified:
 *
 */

#include	<stdio.h>

beep()
{
	if (putchar('\007') != EOF)
		(void)fflush(stdout);
}
