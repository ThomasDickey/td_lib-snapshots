#ifndef	lint
static	char	sccs_id[] = "@(#)rcstemp.c	1.1 88/08/25 16:22:13";
#endif	lint

/*
 * Title:	rcstemp.c (rcs to temp-name)
 * Author:	T.E.Dickey
 * Created:	25 Aug 1988
 * Modified:
 *
 * Function:	If we are run in setuid-mode, we must be able to extract and
 *		otherwise manipulate the file into a place where 'co' and 'ci'
 *		will not fail because of 'access()' checks -- which checks
 *		the real uid.
 */

#include	<stdio.h>
#include	<errno.h>
extern	char	*strcat();
extern	char	*strcpy();

char *
rcstemp(working)
char	*working;
{
	extern	char	*pathleaf();
	static	char	bfr[BUFSIZ];
	if (getuid() != geteuid()) {
		working = strcat(strcpy(bfr, "/tmp/"), pathleaf(working));
		errno = 0;
		if (unlink(working) < 0 && errno != ENOENT)
			failed(working);
	}
	return (working);
}
