/*
 * Returns a positive number if the given argument corresponds to a valid
 * group-id.
 * (If an integer is given, no checking is done).
 */

#include	<grp.h>

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
