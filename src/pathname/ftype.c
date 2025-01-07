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

MODULE_ID("$Id: ftype.c,v 12.7 2025/01/07 00:30:52 tom Exp $")

char *
ftype(char *path)
{
    char *s, *t;

    if ((s = fleaf(path)) == NULL)	/* find leaf */
	s = path;
    if ((t = strchr(s, '.')) == NULL)	/* ...and suffix in leaf */
	t = s + strlen(s);
    return (t);
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
