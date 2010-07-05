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

MODULE_ID("$Id: track_wd.c,v 12.6 2010/07/04 09:39:12 tom Exp $")

void
track_wd(const char *path)
{
    static char current[BUFSIZ];
    static char original[BUFSIZ];

    if (path == 0) {		/* initialization-call */
	if (getwd(original) == 0)
	    strcpy(original, ".");
    } else if (strcmp(path, current)) {
	const char *ddir = ((strlen(current) > strlen(path))
			    ? "return to"
			    : "process");
	PRINTF("** %s %s\n", ddir, relpath(current, original, path));
	(void) strcpy(current, path);
    }
}
