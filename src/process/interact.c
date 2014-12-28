/*
 * Title:	interactive.c
 * Author:	T.E.Dickey
 * Created:	04 Feb 1992
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *
 * Function:	returns true iff the process is running interactively (i.e.,
 *		non-batch).
 */

#define OPN_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: interact.c,v 12.6 2014/12/28 01:11:01 tom Exp $")

int
interactive(void)
{
    static int init = FALSE;
    static int opened_tty;

    if (!init) {
	init = TRUE;
	if ((opened_tty = open("/dev/tty", 0, 0)) >= 0)
	    (void) close(opened_tty);
    }
    return opened_tty >= 0;
}

/******************************************************************************/
#ifdef	TEST
_MAIN
{
    (void) argc;
    (void) argv;
    exit(EXIT_FAILURE);
    /*NOTREACHED */
}
#endif /* TEST */
