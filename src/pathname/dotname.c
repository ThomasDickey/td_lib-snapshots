/*
 * Title:	dotname.c
 * Author:	T.E.Dickey
 * Created:	10 Dec 1985
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		16 Dec 1985, oops: wrong return on "../n" forms.
 */

#include "ptypes.h"

MODULE_ID("$Id: dotname.c,v 12.4 2004/03/07 22:03:45 tom Exp $")

int
dotname(char *path)
{
    if (*path++ == '.') {
	if (*path == '.')
	    path++;
	return (*path == EOS);
    } else
	return (0);
}
