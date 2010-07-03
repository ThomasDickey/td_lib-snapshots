/*
 * Title:	stat_file.c
 * Author:	T.E.Dickey
 * Created:	06 Feb 1992
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	does a 'stat()' on a pathname.  If no error is found, it also
 *		tests to ensure the pathname was a regular file, setting an
 *		error-code if not.
 */

#define	ERR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: stat_fil.c,v 12.6 2010/07/03 16:03:13 tom Exp $")

int
stat_file(const char *path, Stat_t * sb)
{
    if (stat(path, sb) >= 0) {
	if (isFILE(sb->st_mode))
	    return 0;
	errno = EISDIR;
    }
    return -1;
}
