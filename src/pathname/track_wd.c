/*
 * Title:	trace_wd.c (track working-directory)
 * Author:	T.E.Dickey
 * Created:	25 Oct 1989
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 * Function:	This is an auxiliary procedure called by applications using
 *		'walktree()' to trace the resulting working-directory.
 */

#define	STR_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: track_wd.c,v 12.4 2004/03/07 22:03:45 tom Exp $")

void
track_wd(char *path)
{
    static char current[BUFSIZ];
    static char original[BUFSIZ];

    if (path == 0)		/* initialization-call */
	(void) getwd(original);
    else if (strcmp(path, current)) {
	char *ddir = (strlen(current) > strlen(path))
	? "return to" : "process";
	PRINTF("** %s %s\n", ddir, relpath(current, original, path));
	(void) strcpy(current, path);
    }
}
