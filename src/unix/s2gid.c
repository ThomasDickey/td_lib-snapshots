#ifndef	lint
static	char	sccs_id[] = "@(#)s2gid.c	1.2 87/11/24 09:42:43";
#endif	lint

/*
 * Title:	s2gid.c (string to gid)
 * Author:	T.E.Dickey
 * Created:	16 Nov 1987
 * Modified:
 *
 * Function:	Returns a positive number if the given argument corresponds to a
 *		valid group-id.  (If an integer is given, no checking is done).
 */

#include	<grp.h>
extern	long	strtol();

s2gid(s)
char	*s;
{
char	*d;
long	val = strtol(s, &d, 0);
	if (*d) {
	register struct	group	*p;
		if (p = getgrnam(s))
			val = p->gr_gid;
		else	val = -1;
	}
	return ((int)val);
}
