#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: stat_dir.c,v 12.2 1994/05/21 20:18:44 tom Exp $";
#endif

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

#include "ptypes.h"
#include <errno.h>

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
