/*
 * Title:	strcount.c
 * Author:	T.E.Dickey
 * Created:	10 Feb 1992
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	counts occurrences of a given character within a string.
 */

#include "ptypes.h"

MODULE_ID("$Id: strcount.c,v 12.4 2014/12/28 01:11:07 tom Exp $")

int
strcount(char *s, int c)
{
    int count = 0;
    if (s != 0)
	while (*s != EOS)
	    if (*s++ == c)
		count++;
    return count;
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
