#ifndef	lint
static	char	Id[] = "$Id: ftype2.c,v 12.0 1991/10/04 13:45:44 ste_cm Rel $";
#endif

/*
 * Title:	ftype2.c (file-type/suffix #2)
 * Author:	T.E.Dickey
 * Created:	13 Sep 1988
 * Modified:
 *		04 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *
 * Function:	returns a pointer to the right-most "."-suffix of the leaf of
 *		the given pathname.
 */

#define	STR_PTYPES
#include	"ptypes.h"

char *
ftype2(
_AR1(char *,	path))
_DCL(char *,	path)
{
	register char	*s,
			*t;

	if ((s = strrchr(path, '/')) == 0)	/* find leaf */
		s = path;
	else	
		s++;
	if ((t = strrchr(s, '.')) == 0)		/* ...and suffix in leaf */
		t = s + strlen(s);
	return (t);
}
