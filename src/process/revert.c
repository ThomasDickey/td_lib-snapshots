#ifndef	lint
static	char	Id[] = "$Id: revert.c,v 5.0 1989/10/04 12:17:10 ste_cm Rel $";
#endif	lint

/*
 * Title:	revert.c
 * Author:	T.E.Dickey
 * Created:	08 Mar 1989
 * $Log: revert.c,v $
 * Revision 5.0  1989/10/04 12:17:10  ste_cm
 * BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *
 *		Revision 4.1  89/10/04  12:17:10  dickey
 *		lint (apollo SR10.1)
 *		
 *		Revision 4.0  89/03/08  13:38:01  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  89/03/08  13:38:01  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  89/03/08  13:38:01  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  89/03/08  13:38:01  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Resets uid and/or gid to disable setuid-action.  This is done
 *		to prevent specific security violations from occurring in
 *		an application program.
 */

#include	"ptypes.h"

#define	TELL	if (msg) fprintf(stderr,

revert(msg)
char	*msg;
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
