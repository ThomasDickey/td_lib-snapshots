#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/cm_funcs/RCS/rcsperm.c,v 1.4 1989/03/13 13:13:04 dickey Exp $";
#endif	lint

/*
 * Title:	rcspermit.c
 * Author:	T.E.Dickey
 * Created:	08 Mar 1989
 * $Log: rcsperm.c,v $
 * Revision 1.4  1989/03/13 13:13:04  dickey
 * sccs2rcs keywords
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
	auto	int	ok	= FALSE;

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
				if (!strcmp(tmp, user)) {
					ok = header = TRUE;
					*tmp = EOS;
					if (base != 0)
						(void)strcpy(base, tip);
				}
			} while (*tmp);
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
