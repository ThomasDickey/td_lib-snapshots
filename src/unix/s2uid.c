#ifndef	lint
static	char	sccs_id[] = "@(#)s2uid.c	1.2 87/11/24 09:43:39";
#endif	lint

/*
 * Title:	s2uid.c (string to uid)
 * Author:	T.E.Dickey
 * Created:	16 Nov 1987
 * Modified:
 *
 * Function:	Returns a positive number if the given argument corresponds to a
 *		valid user-id.  (If an integer is given, no checking is done).
 */

#include	<pwd.h>
extern	long	strtol();

s2uid(s)
char	*s;
{
char	*d;
long	val = strtol(s, &d, 0);
	if (*d) {
	register struct	passwd	*p;
		if (p = getpwnam(s))
			val = p->pw_uid;
		else	val = -1;
	}
	return ((int)val);
}
