/*
 * Title:	next_version.c (RCS/SCCS next-version computation)
 * Author:	T.E.Dickey
 * Created:	01 Oct 1991
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *
 * Function:	Compute the next version number along a branch.
 */

#define		STR_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: next_ver.c,v 12.6 2004/03/07 16:31:58 tom Exp $")

void
next_version(char *dst, char *src)
{
    char *t;

    *dst = EOS;
    while ((t = strchr(src, '.')) != NULL) {
	size_t len = (size_t) (t - src) + 1;
	(void) strncpy(dst, src, len);
	dst += len;
	src = ++t;
    }
    FORMAT(dst, "%ld", strtol(src, &t, 0) + 1);
}
