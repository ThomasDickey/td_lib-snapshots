#ifndef	lint
static	char	*Id = "$Id: interact.c,v 10.1 1992/02/04 13:10:41 dickey Exp $";
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
			close(opened_tty);
	}
	return opened_tty >= 0;
}
