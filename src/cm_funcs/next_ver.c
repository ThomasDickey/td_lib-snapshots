#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: next_ver.c,v 12.3 1993/11/27 14:27:06 dickey Exp $";
#endif

/*
 * Title:	next_version.c (RCS/SCCS next-version computation)
 * Author:	T.E.Dickey
 * Created:	01 Oct 1991
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *
 * Function:	Compute the next version number along a branch.
 */

#define		STR_PTYPES
#include	"ptypes.h"

void	next_version(
	_ARX(char *,	dst)
	_AR1(char *,	src)
		)
	_DCL(char *,	dst)
	_DCL(char *,	src)
{
	char	*t;

	*dst = EOS;
	while ((t = strchr(src, '.')) != NULL) {
		size_t	len = (size_t)(t - src) + 1;
		(void)strncpy(dst, src, len);
		dst += len;
		src = ++t;
	}
	FORMAT(dst, "%d", strtol(src, &t, 0) + 1);
}
