#ifndef	lint
static	char	sccs_id[] = "@(#)rcspermit.c	1.1 89/03/08 13:30:18";
#endif	lint

/*
 * Title:	rcspermit.c
 * Author:	T.E.Dickey
 * Created:	08 Mar 1989
 * Modified:
 *
 * Function:	Verifies that the 'permit' file exists within the specified
 *		RCS directory, and that the user specified is the one running
 *		the checkin/checkout program.
 *
 *		The permit file contains zero or more records formatted
 *
 *			uid,gid<TAB>rcs_base<NEWLINE>
 *
 *		The "uid,gid" field may be "*,*" (a special case in which all
 *		uids match).
 */

#include	"ptypes.h"
#include	"rcsdefs.h"
#include	<ctype.h>
#include	<string.h>
extern	char	*getenv();
extern	char	*pathcat();
extern	char	*strtrim();

rcspermit(path,base)
char	*path,*base;
{
	auto	char	bfr[BUFSIZ],
			get[80],
			*s;
	static	char	all[]	= "*,*\t";
	auto	FILE	*fp;
	auto	int	ok	= FALSE,
			len;

	path = pathcat(bfr, path, VCS_FILE);

	/*
	 * Reset caller's copy of $RCS_BASE in case we are processing more than
	 * one RCS directory.
	 */
	if (s = getenv("RCS_BASE"))
		(void)strcpy(base, s);
	else
		*base = EOS;

	/*
	 * If we find the caller's uid+gid combination in the permission file,
	 * set the corresponding copy of $RCS_BASE and return true.
	 */
	if (fp = fopen(path, "r")) {
		(void)sprintf(get, "%d,%d\t", getuid(), getgid());
		while (fgets(bfr, sizeof(bfr), fp)) {
			if (!strncmp(bfr, get, len = strlen(get))
			||  !strncmp(bfr, all, len = strlen(all))) {
				ok = TRUE;
				if (base)
					(void)strcpy(base, strtrim(bfr+len));
				break;
			}
		}
		(void)fclose(fp);
	}
	return(ok);
}
