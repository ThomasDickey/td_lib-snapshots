#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/process/RCS/getuser.c,v 4.0 1988/08/10 10:49:00 ste_cm Rel $";
#endif	lint

/*
 * Title:	getuser.c (get user name)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * $Log: getuser.c,v $
 * Revision 4.0  1988/08/10 10:49:00  ste_cm
 * BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
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

#include <pwd.h>

char *
getuser()
{
	extern	struct passwd *getpwuid();		/* cf: apollo sys5 */
	static	struct	passwd p;
	p = *getpwuid(getuid());
	return (p.pw_name);
}
