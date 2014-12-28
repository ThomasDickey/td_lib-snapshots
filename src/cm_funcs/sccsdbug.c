/*
 * Title:	sccs_debug.c (rcs debug-flag)
 * Author:	T.E.Dickey
 * Created:	30 May 1994
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *
 * Function:	Encapsulates knowledge of the SCCS_DEBUG enviromnent variable
 *		so that it can be easily set/cleared from C-shell or Bourne
 *		shell.
 *
 *		We return true iff the variable is both defined and non-null.
 *		If the variable is a numeric constant, we return that value.
 */

#include "ptypes.h"
#include "sccsdefs.h"

MODULE_ID("$Id: sccsdbug.c,v 12.5 2014/12/28 01:10:33 tom Exp $")

int
sccs_debug(void)
{
    static int flag = -1;
    char *s;

    if (flag < 0) {
	flag = 0;
	if ((s = getenv("SCCS_DEBUG")) != 0) {
	    if (*s) {
		char *d;
		flag = (int) strtol(s, &d, 0);
		if (*d)
		    flag = 1;	/* not a number */
	    }
	}
    }
    return (flag);
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
