#ifndef	lint
static	char	*Id = "$Id: interact.c,v 12.0 1992/02/07 15:29:15 ste_cm Rel $";
#endif

/*
 * Title:	interactive.c
 * Author:	T.E.Dickey
 * Created:	04 Feb 1992
 *
 * Function:	returns true iff the process is running interactively (i.e.,
 *		non-batch).
 */

#include "ptypes.h"

int
interactive(_AR0)
{
	static	int	init = FALSE;
	static	int	opened_tty;

	if (!init) {
		init = TRUE;
		if ((opened_tty = open("/dev/tty", 0)) >= 0)
			(void)close(opened_tty);
	}
	return opened_tty >= 0;
}
