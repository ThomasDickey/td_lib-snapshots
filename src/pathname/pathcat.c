#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: pathcat.c,v 12.2 1993/10/29 17:35:25 dickey Exp $";
#endif

/*
 * Title:	pathcat.c (path concatenation)
 * Author:	T.E.Dickey
 * Created:	12 Sep 1988
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		28 Jan 1992, 'dname' may be empty.
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		24 Apr 1990, if fname-argument begins with "~", assume caller
 *			     can expand it ok; simply copy as in '/'-paths.
 *		06 Dec 1989, 'fname' argument may be empty (cf: link2rcs).
 *		04 Oct 1989, lint (apollo SR10.1)
 *		
 * Function:	Forms a "path/filename" string, given the two parts, and
 *		ensures that we treat existing "/" marks intelligently.
 *
 *		Assumes that 'fname' and 'dst' are distinct buffers.  The
 *		'dst' and 'dname' buffers may be the same, though.
 */

#define	STR_PTYPES
#include	"ptypes.h"

char *
pathcat(
_ARX(char *,	dst)
_ARX(char *,	dname)
_AR1(char *,	fname)
	)
_DCL(char *,	dst)
_DCL(char *,	dname)
_DCL(char *,	fname)
{
	auto	char	tmp[BUFSIZ],
			*s;

	if (*fname == '/' || *fname == '~' || !dname || !*dname)
		return (strcpy(dst, fname));
	else if (*fname == EOS) {
		if (dst != dname)
			(void)strcpy(dst, dname);
		return (dst);
	}
	(void)strcpy(tmp, dname);
	if ((s = strrchr(tmp, '/')) && (!strcmp(s, "/")))
		*s = EOS;		/* trim excess '/' */
	return (strcpy(dst, (strcat(strcat(tmp, "/"), fname))));
}
