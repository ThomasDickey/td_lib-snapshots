#ifndef	lint
static	char	Id[] = "$Id: track_wd.c,v 12.1 1993/09/21 18:54:03 dickey Exp $";
#endif

/*
 * Title:	trace_wd.c (track working-directory)
 * Author:	T.E.Dickey
 * Created:	25 Oct 1989
 * Modified:
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 * Function:	This is an auxiliary procedure called by applications using
 *		'walktree()' to trace the resulting working-directory.
 */

#define	STR_PTYPES
#include	"ptypes.h"

void	track_wd(
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
