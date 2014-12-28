/*
 * Title:	fleaf.c (file/pathname leaf)
 * Author:	T.E.Dickey
 * Created:	27 Nov 1993
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *
 * Function:	Returns a pointer in 'path' to the leaf (i.e., filename)
 *		iff a pathname-delimiter precedes it.  If no delimiter is
 *		found, return null.
 *
 *		This is different from 'pathleaf()', which always returns
 *		a nonnull pointer, and properly handles the cases of trailing
 *		pathname delimiters.
 */
#define STR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: fleaf.c,v 12.8 2014/12/28 01:10:51 tom Exp $")

char *
fleaf_delim(const char *path)
{
#ifdef	SYS_UNIX
    return strrchr(path, '/');
#endif
#if	defined(vms) || defined(MSDOS)
    char *s = path + strlen(path);
    while (s-- != path) {
	if (isSlash(*s))
	    return s;
    }
    return 0;
#endif
}

char *
fleaf(const char *path)
{
    char *s = fleaf_delim(path);
    if (s != 0)
	s++;
    return s;
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
