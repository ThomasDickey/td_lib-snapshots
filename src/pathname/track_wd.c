#ifndef	lint
static	char	Id[] = "$Id: track_wd.c,v 12.0 1991/10/04 08:12:58 ste_cm Rel $";
#endif

/*
 * Title:	trace_wd.c (track working-directory)
 * Author:	T.E.Dickey
 * Created:	25 Oct 1989
 * Modified:
 *		03 Oct 1991, converted to ANSI
 * Function:	This is an auxiliary procedure called by applications using
 *		'walktree()' to trace the resulting working-directory.
 */

#define	STR_PTYPES
#include	"ptypes.h"

track_wd(
_AR1(char *,	path))
_DCL(char *,	path)
{
	static	char	current[BUFSIZ];
	static	char	original[BUFSIZ];

	if (path == 0)			/* initialization-call */
		(void)getwd(original);
	else if (strcmp(path, current)) {
		auto	char	*ddir = (strlen(current) > strlen(path))
					? "return to" : "process";
		PRINTF("** %s %s\n", ddir, relpath(current, original, path));
		(void)strcpy(current, path);
	}
}
