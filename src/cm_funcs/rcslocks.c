#ifndef	lint
static	char	Id[] = "$Id: rcslocks.c,v 9.1 1991/10/03 11:49:03 dickey Exp $";
#endif

/*
 * Title:	rcslocks.c (RCS lock-parsing)
 * Author:	T.E.Dickey
 * Created:	24 Aug 1988, from 'checkin'
 * Modified:
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		07 Dec 1989, lint (Apollo SR10.1)
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

#define	STR_PTYPES
#include	"ptypes.h"
#include	"rcsdefs.h"

char *
rcslocks(
_ARX(char *,	s)			/* current scan position */
_ARX(char *,	who)
_AR1(char *,	rev)
	)
_DCL(char *,	s)
_DCL(char *,	who)
_DCL(char *,	rev)
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
