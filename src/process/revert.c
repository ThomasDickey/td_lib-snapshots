/*
 * Title:	revert.c
 * Author:	T.E.Dickey
 * Created:	08 Mar 1989
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		07 Feb 1992, make this return true iff a change is made.
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		07 Dec 1989, lint (SunOs 3.4)
 *		04 Oct 1989, lint (apollo SR10.1)
 *		
 * Function:	Resets uid and/or gid to disable setuid-action.  This is done
 *		to prevent specific security violations from occurring in
 *		an application program.
 */

#include	"ptypes.h"

MODULE_ID("$Id: revert.c,v 12.5 2004/03/07 22:03:45 tom Exp $")

#ifdef	SYS_UNIX

#define	TELL	if (msg) FPRINTF(stderr,

int
revert(char *msg)
{
    int changed = 0;
    uid_t id;
    gid_t gid;
    static char fmt[] = "** reset %s from %d to %d\n";

    if ((id = getuid()) != geteuid()) {
	TELL fmt, "uid", geteuid(), id);
	(void) setuid((int) id);
	changed++;
    }
    if ((gid = getgid()) != getegid()) {
	TELL fmt, "gid", getegid(), gid);
	(void) setgid((int) gid);
	changed++;
    }
    if (changed)
	TELL "** reason: %s\n", msg);
    return changed;
}
#endif /* SYS_UNIX */
