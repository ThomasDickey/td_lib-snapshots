#ifndef	lint
static	char	Id[] = "$Id: getuser.c,v 8.0 1989/10/04 13:11:13 ste_cm Rel $";
#endif	lint

/*
 * Title:	getuser.c (get user name)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * $Log: getuser.c,v $
 * Revision 8.0  1989/10/04 13:11:13  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  89/10/04  13:11:13  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/10/04  13:11:13  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/10/04  13:11:13  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.1  89/10/04  13:11:13  dickey
 *		lint (apollo SR10.1)
 *		
 *		Revision 4.0  88/08/10  10:49:00  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/08/10  10:49:00  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/10  10:49:00  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.3  88/08/10  10:49:00  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Returns the name of the owner of the current process.  Unlike
 *		'getlogin()', it uses the current uid (not the environment).
 */

#include "ptypes.h"
#include <pwd.h>

char *
getuser()
{
	extern	struct passwd *getpwuid();		/* cf: apollo sys5 */
	static	struct	passwd p;
	p = *getpwuid((int)getuid());
	return (p.pw_name);
}
