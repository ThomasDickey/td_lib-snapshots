/*
 * Title:	fleaf14.c
 * Author:	T.E.Dickey
 * Created:	14 Oct 1995
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *
 * Function:	truncates the leaf of the given pathname to 14 characters,
 *		for use with almost-obsolete file systems (and applications
 *		that ought to be obsolete).
 */

#define STR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: fleaf14.c,v 12.4 2014/12/28 01:10:51 tom Exp $")

char *
fleaf14(char *path)
{
    char *leaf = fleaf(path);
    if (leaf == 0)
	leaf = path;
    if (strlen(leaf) > 14)
	leaf[14] = EOS;
    return path;
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
