#ifndef	lint
static	char	Id[] = "$Id: track_wd.c,v 9.0 1991/05/15 09:56:06 ste_cm Rel $";
#endif

/*
 * Title:	trace_wd.c (track working-directory)
 * Author:	T.E.Dickey
 * Created:	25 Oct 1989
 * Function:	This is an auxiliary procedure called by applications using
 *		'walktree()' to trace the resulting working-directory.
 */

#define	STR_PTYPES
#include	"ptypes.h"
extern	char	*relpath();

track_wd(path)
char	*path;
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
