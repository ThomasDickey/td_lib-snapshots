#ifndef	lint
static	char	Id[] = "$Id: next_ver.c,v 10.0 1991/10/17 07:53:33 ste_cm Rel $";
#endif

/*
 * Title:	next_version.c (RCS/SCCS next-version computation)
 * Author:	T.E.Dickey
 * Created:	01 Oct 1991
 * Modified:
 *
 * Function:	Invoke RCS checkin 'ci', then modify the last delta-date of the
 *		corresponding RCS-file to be the same as the modification date
 *
 */

#define		STR_PTYPES
#include	"ptypes.h"

next_version(
_ARX(char *,	dst)
_AR1(char *,	src)
	)
_DCL(char *,	dst)
_DCL(char *,	src)
{
	char	*t;

	*dst = EOS;
	while (t = strchr(src, '.')) {
		size_t	len = t - src + 1;
		(void)strncpy(dst, src, len);
		dst += len;
		src = ++t;
	}
	FORMAT(dst, "%d", strtol(src, &t, 0) + 1);
}
