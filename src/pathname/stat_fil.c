#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: stat_fil.c,v 12.3 1994/08/21 19:31:53 tom Exp $";
#endif

/*
 * Title:	stat_file.c
 * Author:	T.E.Dickey
 * Created:	06 Feb 1992
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	does a 'stat()' on a pathname.  If no error is found, it also
 *		tests to ensure the pathname was a regular file, setting an
 *		error-code if not.
 */

#define	ERR_PTYPES
#include "ptypes.h"

int
stat_file(
_ARX(char *,	path)
_AR1(Stat_t *,	sb)
	)
_DCL(char *,	path)
_DCL(Stat_t *,	sb)
{
	if (stat(path, sb) >= 0) {
		if (isFILE(sb->st_mode))
			return 0;
		errno = EISDIR;
	}
	return -1;
}
