/*
 * Returns a positive number if the given argument corresponds to a valid
 * user-id.
 * (If an integer is given, no checking is done).
 */

#include	<pwd.h>

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
