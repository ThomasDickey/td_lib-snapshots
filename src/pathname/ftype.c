/*
 * Title:	ftype.c (file-type/suffix)
 * Author:	T.E.Dickey
 * Created:	13 Sep 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		04 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		
 * Function:	returns a pointer to the left-most "."-suffix of the leaf of
 *		the given pathname.
 */

#define	STR_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: ftype.c,v 12.4 2004/03/07 22:03:45 tom Exp $")

char *
ftype(char *path)
{
    char *s, *t;

    if ((s = fleaf(path)) == 0)	/* find leaf */
	s = path;
    if ((t = strchr(s, '.')) == 0)	/* ...and suffix in leaf */
	t = s + strlen(s);
    return (t);
}
