/*
 * Title:	stat_dir.c
 * Author:	T.E.Dickey
 * Created:	06 Feb 1992
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	does a 'stat()' on a pathname.  If no error is found, it also
 *		tests to ensure the pathname was a directory file, setting an
 *		error-code if not.
 */

#define	ERR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: stat_dir.c,v 12.4 1994/08/21 19:31:45 tom Exp $")

int
stat_dir(
_ARX(char *,	path)
_AR1(Stat_t *,	sb)
	)
_DCL(char *,	path)
_DCL(Stat_t *,	sb)
{
	if (stat(path, sb) >= 0) {
		if (isDIR(sb->st_mode))
			return 0;
		errno = ENOTDIR;
	}
	return -1;
}
