#ifndef	lint
static	char	Id[] = "$Id: getuser.c,v 12.0 1991/10/03 08:36:15 ste_cm Rel $";
#endif

/*
 * Title:	getuser.c (get user name)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * Modified:
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		04 Oct 1989, lint (apollo SR10.1)
 *
 * Function:	Returns the name of the owner of the current process.  Unlike
 *		'getlogin()', it uses the current uid (not the environment).
 */

#include "ptypes.h"
#include <pwd.h>

char *
getuser(_AR0)
{
	extern	struct passwd *getpwuid();		/* cf: apollo sys5 */
	static	struct	passwd p;
	p = *getpwuid((int)getuid());
	return (p.pw_name);
}
