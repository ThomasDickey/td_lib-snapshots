#ifndef	lint
static	char	sccs_id[] = "@(#)rcstemp.c	1.2 88/08/30 08:38:26";
#endif	lint

/*
 * Title:	rcstemp.c (rcs to temp-name)
 * Author:	T.E.Dickey
 * Created:	25 Aug 1988
 * Modified:
 *		30 Aug 1988, invoke 'filecopy()' to make temp-file look more
 *			     like the working-file.
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
rcstemp(working, copy)
char	*working;
{
	extern	char	*pathleaf();
	static	char	bfr[BUFSIZ];

	if (getuid() != geteuid()) {
		char	*tf = strcat(strcpy(bfr, "/tmp/"), pathleaf(working));
		if (filecopy(working, tf, copy) < 0)
			failed(tf);
		working = tf;
	}
	return (working);
}
