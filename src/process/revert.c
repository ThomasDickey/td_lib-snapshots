#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/process/RCS/revert.c,v 4.0 1989/03/08 13:38:01 ste_cm Rel $";
#endif	lint

/*
 * Title:	revert.c
 * Author:	T.E.Dickey
 * Created:	08 Mar 1989
 * $Log: revert.c,v $
 * Revision 4.0  1989/03/08 13:38:01  ste_cm
 * BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
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

#include	<stdio.h>

#define	TELL	if (msg) fprintf(stderr,

revert(msg)
char	*msg;
{
	register int	changed = 0,
			id;
	static	char	fmt[] = "** reset %s from %d to %d\n";

	if ((id = getuid()) != geteuid()) {
		TELL fmt, "uid", geteuid(), id);
		(void)setuid(id);
		changed++;
	}
	if ((id = getgid()) != getegid()) {
		TELL fmt, "gid", getegid(), id);
		(void)setgid(id);
		changed++;
	}
	if (changed)
		TELL "** reason: %s\n", msg);
}
