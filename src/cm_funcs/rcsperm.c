#ifndef	lint
static	char	Id[] = "$Id: rcsperm.c,v 9.0 1991/05/15 09:46:31 ste_cm Rel $";
#endif

/*
 * Title:	rcspermit.c
 * Author:	T.E.Dickey
 * Created:	08 Mar 1989
 * $Log: rcsperm.c,v $
 * Revision 9.0  1991/05/15 09:46:31  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:46:31  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  89/10/04  11:59:46  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  89/10/04  11:59:46  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/10/04  11:59:46  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/10/04  11:59:46  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.1  89/10/04  11:59:46  dickey
 *		lint (apollo SR10.1)
 *		
 *		Revision 4.0  89/07/25  09:24:30  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.2  89/07/25  09:24:30  dickey
 *		recompiled with apollo SR10 -- mods for function prototypes
 *		
 *		Revision 3.1  89/07/05  09:48:44  dickey
 *		if the access-list is empty, we must own the file to operate
 *		upon it.
 *		
 *		Revision 3.0  89/04/04  11:01:24  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
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

#define	STR_PTYPES
#include	"ptypes.h"
#include	"rcsdefs.h"
#include	<ctype.h>
extern	char	*getenv();
extern	char	*uid2s();
extern	char	*vcs_file();

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

	if (!rcsopen(path, FALSE))
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
