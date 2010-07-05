/*
 * Title:	s2gid.c (string to gid)
 * Author:	T.E.Dickey
 * Created:	16 Nov 1987
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991,
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 * Function:	Returns a positive number if the given argument corresponds to a
 *		valid group-id.  (If an integer is given, no checking is done).
 */

#include	"ptypes.h"

MODULE_ID("$Id: s2gid.c,v 12.7 2010/07/04 09:27:26 tom Exp $")

#ifdef	SYS_UNIX
#include	<grp.h>

int
s2gid(const char *s)
{
    char *d;
    long val = strtol(s, &d, 0);

    if (*d) {
	struct group *p;

	if ((p = getgrnam(s)) != 0)
	    val = p->gr_gid;
	else
	    val = -1;
    }
    return ((int) val);
}
#endif /* SYS_UNIX */
