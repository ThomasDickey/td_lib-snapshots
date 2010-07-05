/*
 * Title:	getuser.c (get user name)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		04 Oct 1989, lint (apollo SR10.1)
 *
 * Function:	Returns the name of the owner of the current process.  Unlike
 *		'getlogin()', it uses the current uid (not the environment).
 */

#define PWD_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: getuser.c,v 12.7 2010/07/04 09:14:53 tom Exp $")

#ifdef	SYS_UNIX
char *
getuser(void)
{
    static struct passwd p;
    p = *getpwuid(getuid());
    return (p.pw_name);
}
#endif /* SYS_UNIX */
