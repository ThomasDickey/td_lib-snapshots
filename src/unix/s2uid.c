#ifndef	lint
static	char	Id[] = "$Id: s2uid.c,v 8.0 1990/05/15 07:56:27 ste_cm Rel $";
#endif	lint

/*
 * Title:	s2uid.c (string to uid)
 * Author:	T.E.Dickey
 * Created:	16 Nov 1987
 * $Log: s2uid.c,v $
 * Revision 8.0  1990/05/15 07:56:27  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.1  90/05/15  07:56:27  dickey
 *		added a hack which reads the apollo passwd-file directly to
 *		bypass a bug in their handling of obsolete accounts.
 *		
 *		Revision 7.0  88/08/10  11:12:24  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/08/10  11:12:24  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/08/10  11:12:24  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/08/10  11:12:24  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/08/10  11:12:24  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/10  11:12:24  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.4  88/08/10  11:12:24  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Returns a positive number if the given argument corresponds to a
 *		valid user-id.  (If an integer is given, no checking is done).
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	<pwd.h>
extern	long	strtol();

#ifdef	apollo_sr10
	/*
	 * Cover up bug in Apollo SR10.2 which causes the passwd-procedures to
	 * not return data for a user whose account is deleted.
	 */
extern	int	len_passwd;	/* share with 'uid2s.c' */
extern	char	**vec_passwd;
static	unknown_uid(s)
	char	*s;
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

s2uid(s)
char	*s;
{
char	*d;
long	val = strtol(s, &d, 0);
	if (*d) {
	extern	 struct	passwd	*getpwnam();	/* cf: apollo sys5 */
	register struct	passwd	*p;
		if (p = getpwnam(s))
			val = p->pw_uid;
		else	val = unknown_uid(s);
	}
	return ((int)val);
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
	register int	j;
	for (j = 1; j < argc; j++)
		PRINTF("%s => %d\n", argv[j], s2uid(argv[j]));
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
