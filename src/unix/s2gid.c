#ifndef	lint
static	char	Id[] = "$Id: s2gid.c,v 12.0 1991/10/03 08:09:40 ste_cm Rel $";
#endif

/*
 * Title:	s2gid.c (string to gid)
 * Author:	T.E.Dickey
 * Created:	16 Nov 1987
 * Modified:
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991,
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 * Function:	Returns a positive number if the given argument corresponds to a
 *		valid group-id.  (If an integer is given, no checking is done).
 */

#include	"ptypes.h"
#include	<grp.h>

s2gid(
_AR1(char *,	s))
_DCL(char *,	s)
{
char	*d;
long	val = strtol(s, &d, 0);
	if (*d) {
	extern	 struct	group	*getgrnam();	/* cf: apollo sys5 */
	register struct	group	*p;
		if (p = getgrnam(s))
			val = p->gr_gid;
		else	val = -1;
	}
	return ((int)val);
}
