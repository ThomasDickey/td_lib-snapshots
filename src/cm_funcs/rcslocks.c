#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/cm_funcs/RCS/rcslocks.c,v 5.0 1988/08/24 11:59:03 ste_cm Rel $";
#endif	lint

/*
 * Title:	rcslocks.c (RCS lock-parsing)
 * Author:	T.E.Dickey
 * Created:	24 Aug 1988, from 'checkin'
 * $Log: rcslocks.c,v $
 * Revision 5.0  1988/08/24 11:59:03  ste_cm
 * BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *
 *		Revision 4.0  88/08/24  11:59:03  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/08/24  11:59:03  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/24  11:59:03  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  88/08/24  11:59:03  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Within the 'rcsedit' state S_LOCKS, parse for information to
 *		complete our knowledge of the locks on an RCS file.  This is
 *		filled in according to which arguments are non-null when this
 *		is invoked:
 *
 *		who	=> obtain revision which is locked
 *		rev	=> obtain user who has locked it
 *		<nil>	=> obtain any lock; override if we find one for the
 *			   current user.
 *
 * Returns:	the scan position past the last lock; the arguments are updated
 *		to hold the appropriate lock information.
 */

#include	"ptypes.h"
#include	"rcsdefs.h"
extern	char	*getuser();
extern	char	*rcsparse_id(), *rcsparse_num();
extern	char	*strcpy();

char *
rcslocks(s, who, rev)
char	*s;			/* current scan position */
char	*who;
char	*rev;
{
	int	any_one	= (*who == EOS),
		any_rev = (*rev == EOS);
	char	locked_by[BUFSIZ],
		lock_name[BUFSIZ];
	char	*who_am_i = *who ? who : getuser();

	do {
		s = rcsparse_id(locked_by, s);
		if (*s == ':')	s++;
		s = rcsparse_num(lock_name, s);
		if (*locked_by && *lock_name) {
			if (any_one && (any_rev || !strcmp(rev, lock_name)))
				(void)strcpy(who, locked_by);
			if (any_rev && (any_one || !strcmp(who, locked_by)))
				(void)strcpy(rev, lock_name);
			if (!strcmp(locked_by, who_am_i))
				any_one =
				any_rev = FALSE;
		}
	} while (*locked_by);
	return (s);
}
