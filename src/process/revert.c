#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: revert.c,v 12.2 1993/11/26 22:24:28 dickey Exp $";
#endif

/*
 * Title:	revert.c
 * Author:	T.E.Dickey
 * Created:	08 Mar 1989
 * Modified:
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

#ifdef	unix

#define	TELL	if (msg) FPRINTF(stderr,

int
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
		(void)setuid((int)id);
		changed++;
	}
	if ((gid = getgid()) != getegid()) {
		TELL fmt, "gid", getegid(), gid);
		(void)setgid((int)gid);
		changed++;
	}
	if (changed)
		TELL "** reason: %s\n", msg);
	return changed;
}
#endif	/* unix */
