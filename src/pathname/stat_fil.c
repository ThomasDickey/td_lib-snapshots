#ifndef	lint
static	char	*Id = "$Id: stat_fil.c,v 12.0 1992/02/06 08:38:34 ste_cm Rel $";
#endif

/*
 * Title:	stat_file.c
 * Author:	T.E.Dickey
 * Created:	06 Feb 1992
 *
 * Function:	does a 'stat()' on a pathname.  If no error is found, it also
 *		tests to ensure the pathname was a regular file, setting an
 *		error-code if not.
 */

#include "ptypes.h"
#include <errno.h>

int
stat_file(
_ARX(char *,	path)
_AR1(STAT *,	sb)
	)
_DCL(char *,	path)
_DCL(STAT *,	sb)
{
	if (stat(path, sb) >= 0) {
		if (isFILE(sb->st_mode))
			return 0;
		errno = EISDIR;
	}
	return -1;
}
