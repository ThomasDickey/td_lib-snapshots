#ifndef	lint
static	char	Id[] = "$Id: revert.c,v 10.0 1991/10/03 08:10:57 ste_cm Rel $";
#endif

/*
 * Title:	revert.c
 * Author:	T.E.Dickey
 * Created:	08 Mar 1989
 * Modified:
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

#define	TELL	if (msg) FPRINTF(stderr,

revert(
_AR1(char *,	msg))
_DCL(char *,	msg)
{
	register int	changed = 0;
	register uid_t	id;
	register gid_t	gid;
	static	char	fmt[] = "** reset %s from %d to %d\n";

	if ((id = getuid()) != geteuid()) {
		TELL fmt, "uid", geteuid(), id);
		(void)setuid(id);
		changed++;
	}
	if ((gid = getgid()) != getegid()) {
		TELL fmt, "gid", getegid(), gid);
		(void)setgid(gid);
		changed++;
	}
	if (changed)
		TELL "** reason: %s\n", msg);
}
