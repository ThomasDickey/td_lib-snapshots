#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/unix/RCS/s2gid.c,v 8.0 1988/08/10 11:11:48 ste_cm Rel $";
#endif	lint

/*
 * Title:	s2gid.c (string to gid)
 * Author:	T.E.Dickey
 * Created:	16 Nov 1987
 * $Log: s2gid.c,v $
 * Revision 8.0  1988/08/10 11:11:48  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  88/08/10  11:11:48  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/08/10  11:11:48  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/08/10  11:11:48  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/08/10  11:11:48  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/08/10  11:11:48  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/10  11:11:48  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.4  88/08/10  11:11:48  dickey
 *		sccs2rcs keywords
 *		
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
	extern	 struct	group	*getgrnam();	/* cf: apollo sys5 */
	register struct	group	*p;
		if (p = getgrnam(s))
			val = p->gr_gid;
		else	val = -1;
	}
	return ((int)val);
}
