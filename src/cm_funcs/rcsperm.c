#ifndef	lint
static	char	Id[] = "$Id: rcsperm.c,v 12.1 1993/09/21 18:54:03 dickey Exp $";
#endif

/*
 * Title:	rcspermit.c
 * Author:	T.E.Dickey
 * Created:	08 Mar 1989
 * Modified:
 *		21 Sep 1993, gcc-warnings
 *		12 Nov 1992, added 'accflag' argument.
 *		04 Oct 1991, conversion to ANSI
 *		06 Sep 1991, modified interface to 'rcsopen()'
 *		05 Jul 1989, if the access-list is empty, we must own the file
 *			     to operate upon it.
 *		04 Apr 1989, if access list of permit-file is empty, assume
 *			     that we are using it solely for the baseline-
 *			     version.  In this case, return the baseline
 *			     version just as we would for permission checking.
 *		
 *		10 Mar 1989, rewrote using 'rcsedit' module rather than special-
 *			     purpose file.
 *
 * Function:	Verifies that the 'permit' file exists within the specified
 *		RCS directory, and that the user specified is the one running
 *		the checkin/checkout program.
 *
 *		The permit file is an RCS file which records only the baseline
 *		information.
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	"rcsdefs.h"
#include	"dyn_str.h"
#include	<ctype.h>

int	rcspermit(
	_ARX(char *,	path)
	_ARX(char *,	base)
	_AR1(char **,	accflag)
		)
	_DCL(char *,	path)
	_DCL(char *,	base)
	_DCL(char **,	accflag)
{
	static	DYN *	access_list;
	auto	STAT	sb;
	auto	int	header	= TRUE;
	auto	char	*s	= 0,
			tip	[80],
			user	[BUFSIZ],
			key	[BUFSIZ],
			tmp	[BUFSIZ];
	auto	int	empty	= TRUE,		/* assume access-list empty */
			my_file,
			code	= S_FAIL,
			ok	= FALSE;	/* assume no permission */

	path = vcs_file(path, tmp, FALSE);
	dyn_init(&access_list, BUFSIZ);

	/*
	 * Reset caller's copy of $RCS_BASE in case we are processing more than
	 * one RCS directory.
	 */
	if (base != 0) {
		register char *t;
		if ((t = getenv("RCS_BASE")) != NULL)
			(void)strcpy(base, t);
		else
			*base = EOS;
	}

	/*
	 * If we find the caller's uid/gid combination in the permission file,
	 * (or if the access list is blank)
	 * set the corresponding copy of $RCS_BASE and return true.
	 */
	(void)strcpy (user, uid2s((int)getuid()));

	if (!rcsopen(path, RCS_DEBUG, TRUE))
		return (FALSE);		/* could not open file anyway */

	/*
	 * If access-list is empty, we want to know who owns the file:
	 */
	my_file = ((stat(path, &sb) >= 0)	/* ok always! */
		&& (sb.st_uid == getuid()));

	while (header && (s = rcsread(s, code))) {
		s = rcsparse_id(key, s);

		switch (code = rcskeys(key)) {
		case S_HEAD:
			s = rcsparse_num(tip, s);
			break;
		case S_ACCESS:
			for(;;) {
				s = rcsparse_id(tmp,s);
				if (*tmp == EOS)
					break;
				if (!empty)
					APPEND(access_list, ",");
				APPEND(access_list, tmp);
				empty = FALSE;
				if (!ok)
					ok = !strcmp(tmp, user);
			}
			if (empty) {
				ok = my_file;
			}
			if ((header = ok)
			&&  (base  != 0))
				(void)strcpy(base, tip);
			break;
		case S_LOCKS:
			s = rcslocks(s, strcpy(key, user), tmp);
			break;
		case S_VERS:
			header = FALSE;
			break;
		}
	}
	rcsclose();
	if (accflag != 0)
		*accflag = txtalloc(dyn_string(access_list));
	return(ok);
}
