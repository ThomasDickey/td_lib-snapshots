/*
 * Title:	rcslocks.c (RCS lock-parsing)
 * Author:	T.E.Dickey
 * Created:	24 Aug 1988, from 'checkin'
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
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
 *		(both)	=> obtain any lock; override if we find one for the
 *			   current user.
 *
 * Returns:	the scan position past the last lock; the arguments are updated
 *		to hold the appropriate lock information.
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	"rcsdefs.h"

MODULE_ID("$Id: rcslocks.c,v 12.4 2014/12/28 01:10:33 tom Exp $")

char *
rcslocks(char *s,		/* current scan position */
	 char *who,
	 char *rev)
{
    int any_one = (*who == EOS);
    int any_rev = (*rev == EOS);
    char locked_by[BUFSIZ];
    char lock_name[BUFSIZ];
    char *who_am_i = *who ? who : getuser();

    do {
	s = rcsparse_id(locked_by, s);
	if (*s == ':')
	    s++;
	s = rcsparse_num(lock_name, s);
	if (*locked_by && *lock_name) {
	    if (any_one && (any_rev || !strcmp(rev, lock_name)))
		(void) strcpy(who, locked_by);
	    if (any_rev && (any_one || !strcmp(who, locked_by)))
		(void) strcpy(rev, lock_name);
	    if (!strcmp(locked_by, who_am_i))
		any_one =
		    any_rev = FALSE;
	}
    } while (*locked_by);
    return (s);
}

/******************************************************************************/
#ifdef	TEST
_MAIN
{
    (void) argc;
    (void) argv;
    exit(EXIT_FAILURE);
    /*NOTREACHED */
}
#endif /* TEST */
