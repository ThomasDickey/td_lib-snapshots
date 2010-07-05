/*
 * Title:	samepath.c (compare pathname prefixes)
 * Author:	T.E.Dickey
 * Created:	13 Jul 1994
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		30 Apr 2003, correct split-out (check for slash or null).
 *		25 Apr 2003, split-out from sccs_dir.c
 *		
 * Compare two pathnames, returning the length of the matching portion,
 * limited to a pathname separator.  Note that 'strchr()' can use a null
 * character for the second argument.
 */

#define	STR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: samehead.c,v 12.5 2010/07/04 09:26:50 tom Exp $")

int
samehead(const char *path1, const char *path2)
{
    int match = 0;
    int n;

    for (n = 0;; n++) {
	if ((isSlash(path1[n]) || path1[n] == EOS)
	    && (isSlash(path2[n]) || path2[n] == EOS))
	    match = n;
	if (path1[n] == EOS || path2[n] == EOS)
	    break;
	if (path1[n] != path2[n])
	    break;
    }
    return match;
}
