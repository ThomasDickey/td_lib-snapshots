#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/cm_funcs/RCS/rcspath.c,v 8.0 1989/07/25 15:56:39 ste_cm Rel $";
#endif	lint

/*
 * Title:	rcspath.c (rcs-utility to path-name)
 * Author:	T.E.Dickey
 * Created:	09 Sep 1988
 * $Log: rcspath.c,v $
 * Revision 8.0  1989/07/25 15:56:39  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  89/07/25  15:56:39  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/07/25  15:56:39  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/07/25  15:56:39  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  89/07/25  15:56:39  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.1  89/07/25  15:56:39  dickey
 *		use 'pathcat()' so we don't have to worry about supplying trailing '/' in
 *		the RCS_PATH definition.
 *		
 *		Revision 3.0  89/03/27  14:15:17  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
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
extern	char	*pathcat();

char *
rcspath(utility)
char	*utility;
{
#ifdef	RCS_PATH
	static	char	bfr[BUFSIZ];
	return pathcat(bfr, RCS_PATH, utility);
#else	RCS_PATH
	return (utility);
#endif	RCS_PATH
}
