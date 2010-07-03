/*
 * Title:	stat_dir.c
 * Author:	T.E.Dickey
 * Created:	06 Feb 1992
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	does a 'stat()' on a pathname.  If no error is found, it also
 *		tests to ensure the pathname was a directory file, setting an
 *		error-code if not.
 */

#define	ERR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: stat_dir.c,v 12.6 2010/07/03 16:02:53 tom Exp $")

int
stat_dir(const char *path, Stat_t * sb)
{
    if (stat(path, sb) >= 0) {
	if (isDIR(sb->st_mode))
	    return 0;
	errno = ENOTDIR;
    }
    return -1;
}
