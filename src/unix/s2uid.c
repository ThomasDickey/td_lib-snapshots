#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/unix/RCS/s2uid.c,v 7.0 1988/08/10 11:12:24 ste_cm Rel $";
#endif	lint

/*
 * Title:	s2uid.c (string to uid)
 * Author:	T.E.Dickey
 * Created:	16 Nov 1987
 * $Log: s2uid.c,v $
 * Revision 7.0  1988/08/10 11:12:24  ste_cm
 * BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
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

#include	<pwd.h>
extern	long	strtol();

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
		else	val = -1;
	}
	return ((int)val);
}
