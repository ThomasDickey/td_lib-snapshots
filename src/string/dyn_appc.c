/*
 * Title:	dyn_append_c.c
 * Author:	T.E.Dickey
 * Created:	24 Jul 1992
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	appends a character to a dynamic-string.
 */

#define	STR_PTYPES
#include "ptypes.h"
#include "dyn_str.h"

MODULE_ID("$Id: dyn_appc.c,v 12.5 2014/12/28 01:11:07 tom Exp $")

DYN *
dyn_append_c(DYN * p, int c)
{
    static char temp[] = "?";
    if (c != EOS) {
	temp[0] = (char) c;
	p = dyn_append(p, temp);
    }
    return p;
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
