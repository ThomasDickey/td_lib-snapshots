#ifndef	lint
static	char	sccs_id[] = "@(#)rcspath.c	1.1 88/09/09 09:28:22";
#endif	lint

/*
 * Title:	rcspath.c (rcs-utility to path-name)
 * Author:	T.E.Dickey
 * Created:	09 Sep 1988
 * Modified:
 *
 * Function:	This returns the pathname of the specified RCS-utility, and
 *		is intended to provide a measure of security in executing
 *		the utilities from a setuid program.
 */

#include	<stdio.h>
extern	char	*strcat();
extern	char	*strcpy();

#ifndef	RCS_PATH
#ifdef	apollo
#define	RCS_PATH	"//spe_site/local/ste/bin/"
#endif	apollo
#endif	RCS_PATH

char *
rcspath(utility)
char	*utility;
{
#ifdef	RCS_PATH
	static	char	bfr[BUFSIZ];
	return (strcat(strcpy(bfr, RCS_PATH), utility));
#else	RCS_PATH
	return (utility);
#endif	RCS_PATH
}
