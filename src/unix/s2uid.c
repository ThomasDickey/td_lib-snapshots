#ifndef	lint
static	char	Id[] = "$Id: s2uid.c,v 12.1 1993/09/21 18:54:03 dickey Exp $";
#endif

/*
 * Title:	s2uid.c (string to uid)
 * Author:	T.E.Dickey
 * Created:	16 Nov 1987
 * Modified:
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		15 May 1990, added a hack which reads the apollo passwd-file
 *			     directly to bypass a bug in their handling of
 *			     obsolete accounts.
 *		
 * Function:	Returns a positive number if the given argument corresponds to a
 *		valid user-id.  (If an integer is given, no checking is done).
 */

#define	PWD_PTYPES
#define	STR_PTYPES
#include "ptypes.h"

#ifdef	apollo_sr10
	/*
	 * Cover up bug in Apollo SR10.2 which causes the passwd-procedures to
	 * not return data for a user whose account is deleted.
	 */
extern	int	len_passwd;	/* share with 'uid2s.c' */
extern	char	**vec_passwd;
static	unknown_uid(
	_AR1(char *,	s))
	_DCL(char *,	s)
	{
		auto	size_t	len = strlen(s);
		auto	char	*d;
		register int	j;

		if (!len_passwd)
			len_passwd = file2argv("/etc/passwd", &vec_passwd);
		for (j = 0; j < len_passwd; j++) {
			if (vec_passwd[j][len] == ':'
			&&  !strncmp(vec_passwd[j],s,len)) {
				s = vec_passwd[j] + (len + 1);
				while (*s++ != ':');
				return (strtol(s, &d, 0));
			}
		}
		return (-1);
	}
#else
#define	unknown_uid(s)	-1
#endif

int	s2uid(
	_AR1(char *,	s))
	_DCL(char *,	s)
{
	char	*d;
	long	val = strtol(s, &d, 0);

	if (*d) {
		register struct	passwd	*p;
		if ((p = getpwnam(s)) != 0)
			val = p->pw_uid;
		else	val = unknown_uid(s);
	}
	return ((int)val);
}

#ifdef	TEST
_MAIN
{
	register int	j;
	for (j = 1; j < argc; j++)
		PRINTF("%s => %d\n", argv[j], s2uid(argv[j]));
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
