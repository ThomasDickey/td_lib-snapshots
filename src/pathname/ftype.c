#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: ftype.c,v 12.1 1993/10/29 17:35:26 dickey Exp $";
#endif

/*
 * Title:	ftype.c (file-type/suffix)
 * Author:	T.E.Dickey
 * Created:	13 Sep 1988
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		04 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		
 * Function:	returns a pointer to the left-most "."-suffix of the leaf of
 *		the given pathname.
 */

#define	STR_PTYPES
#include	"ptypes.h"

#ifdef	vms
#define	PATH_END	']'
#else	/* unix */
#define	PATH_END	'/'
#endif	/* vms/unix */

char *
ftype(
_AR1(char *,	path))
_DCL(char *,	path)
{
	register char	*s,
			*t;

	if ((s = strrchr(path, PATH_END)) == 0)	/* find leaf */
		s = path;
	else	
		s++;
	if ((t = strchr(s, '.')) == 0)		/* ...and suffix in leaf */
		t = s + strlen(s);
	return (t);
}
