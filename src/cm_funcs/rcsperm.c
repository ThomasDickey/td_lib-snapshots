#ifndef	lint
static	char	Id[] = "$Id: rcsperm.c,v 9.1 1991/09/06 11:23:01 dickey Exp $";
#endif

/*
 * Title:	rcspermit.c
 * Author:	T.E.Dickey
 * Created:	08 Mar 1989
 * Modified:
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
#include	<ctype.h>
extern	char	*uid2s();

rcspermit(path,base)
char	*path,*base;
{
	auto	struct stat sb;
	auto	int	header	= TRUE;
	auto	char	*s	= 0,
			tip	[80],
			user	[BUFSIZ],
			key	[BUFSIZ],
			tmp	[BUFSIZ];
	auto	int	empty	= TRUE,		/* assume access-list empty */
			my_file,
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
	(void)strcpy (user, uid2s((int)getuid()));

	if (!rcsopen(path, FALSE, TRUE))
		return (FALSE);		/* could not open file anyway */

	/*
	 * If access-list is empty, we want to know who owns the file:
	 */
	my_file = ((stat(path, &sb) >= 0)	/* ok always! */
		&& (sb.st_uid == getuid()));

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
		case S_COMMENT:
			s = rcsparse_str(s, NULL_FUNC);
			break;
		}
	}
	rcsclose();
	return(ok);
}
