/*
 * Title:	interactive.c
 * Author:	T.E.Dickey
 * Created:	04 Feb 1992
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *
 * Function:	returns true iff the process is running interactively (i.e.,
 *		non-batch).
 */

#define OPN_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: interact.c,v 12.4 1994/05/23 22:26:49 tom Exp $")

int
interactive(_AR0)
{
	static	int	init = FALSE;
	static	int	opened_tty;

	if (!init) {
		init = TRUE;
		if ((opened_tty = open("/dev/tty", 0, 0)) >= 0)
			(void)close(opened_tty);
	}
	return opened_tty >= 0;
}
