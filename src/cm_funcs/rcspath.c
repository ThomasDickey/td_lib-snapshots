#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/cm_funcs/RCS/rcspath.c,v 1.3 1989/03/16 16:23:19 dickey Exp $";
#endif	lint

/*
 * Title:	rcspath.c (rcs-utility to path-name)
 * Author:	T.E.Dickey
 * Created:	09 Sep 1988
 * $Log: rcspath.c,v $
 * Revision 1.3  1989/03/16 16:23:19  dickey
 * sccs2rcs keywords
 *
 *		16 Mar 1989, changed default for site-path
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
#define	RCS_PATH	"/ste_site/ste/bin/"
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
