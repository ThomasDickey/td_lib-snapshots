#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/cm_funcs/RCS/rcsperm.c,v 3.0 1989/04/04 11:01:24 ste_cm Rel $";
#endif	lint

/*
 * Title:	rcspermit.c
 * Author:	T.E.Dickey
 * Created:	08 Mar 1989
 * $Log: rcsperm.c,v $
 * Revision 3.0  1989/04/04 11:01:24  ste_cm
 * BASELINE Mon Jun 19 13:27:01 EDT 1989
 *
 *		Revision 2.0  89/04/04  11:01:24  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.5  89/04/04  11:01:24  dickey
 *		if access list of permit-file is empty, assume that we are
 *		using it solely for the baseline-version.  In this case,
 *		return the baseline version just as we would for permission
 *		checking.
 *		
 *		Revision 1.4  89/03/13  13:13:04  dickey
 *		sccs2rcs keywords
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

#include	"ptypes.h"
#include	"rcsdefs.h"
#include	<ctype.h>
#include	<string.h>
extern	char	*getenv();
extern	char	*pathcat();
extern	char	*strtrim();
extern	char	*uid2s();
extern	char	*vcs_file();

rcspermit(path,base)
char	*path,*base;
{
	auto	int	header	= TRUE;
	auto	char	*s	= 0,
			tip	[80],
			user	[L_cuserid+1],
			key	[BUFSIZ],
			tmp	[BUFSIZ];
	auto	int	empty	= TRUE,		/* assume access-list empty */
			ok	= FALSE;	/* assume no permission */

	path = vcs_file(path, tmp, FALSE);

	/*
	 * Reset caller's copy of $RCS_BASE in case we are processing more than
	 * one RCS directory.
	 */
	if (base != 0) {
		if (s = getenv("RCS_BASE"))
			(void)strcpy(base, s);
		else
			*base = EOS;
	}

	/*
	 * If we find the caller's uid/gid combination in the permission file,
	 * (or if the access list is blank)
	 * set the corresponding copy of $RCS_BASE and return true.
	 */
	(void)strcpy (user, uid2s(getuid()));

	if (!rcsopen(path, FALSE))
		return (FALSE);		/* could not open file anyway */

	while (header && (s = rcsread(s))) {
		s = rcsparse_id(key, s);

		switch (rcskeys(key)) {
		case S_HEAD:
			s = rcsparse_num(tip, s);
			break;
		case S_ACCESS:
			do {
				s = rcsparse_id(tmp,s);
				if (*tmp == EOS)
					break;
				empty = FALSE;
			} while (!(ok = !strcmp(tmp, user)));
			if (empty)
				ok = TRUE;
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
		case S_COMMENT:
			s = rcsparse_str(s, NULL_FUNC);
			break;
		}
	}
	rcsclose();
	return(ok);
}
