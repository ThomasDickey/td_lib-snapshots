#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/cm_funcs/RCS/rcspath.c,v 3.0 1989/03/27 14:15:17 ste_cm Rel $";
#endif	lint

/*
 * Title:	rcspath.c (rcs-utility to path-name)
 * Author:	T.E.Dickey
 * Created:	09 Sep 1988
 * $Log: rcspath.c,v $
 * Revision 3.0  1989/03/27 14:15:17  ste_cm
 * BASELINE Mon Jun 19 13:27:01 EDT 1989
 *
 *		Revision 2.0  89/03/27  14:15:17  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.4  89/03/27  14:15:17  dickey
 *		use 'rcspath.h' to encapsulate the string-constant RCS_PATH
 *		
 *		Revision 1.3  89/03/16  16:23:19  dickey
 *		sccs2rcs keywords
 *		
 *		16 Mar 1989, changed default for site-path
 *
 * Function:	This returns the pathname of the specified RCS-utility, and
 *		is intended to provide a measure of security in executing
 *		the utilities from a setuid program.
 */

#include	<stdio.h>
#include	<string.h>
#include	"rcspath.h"

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
