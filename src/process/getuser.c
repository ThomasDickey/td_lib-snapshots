#ifndef	lint
static	char	sccs_id[] = "@(#)getuser.c	1.1 88/05/26 08:39:27";
#endif	lint

/*
 * Title:	getuser.c (get user name)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * Modified:
 *
 * Function:	Returns the name of the owner of the current process.  Unlike
 *		'getlogin()', it uses the current uid (not the environment).
 */

#include <pwd.h>

char *
getuser()
{
static
struct	passwd p;
	p = *getpwuid(getuid());
	return (p.pw_name);
}
