/*
 * Title:	failed.c (generic exit from program)
 * Author:	T.E.Dickey
 * Created:	27 Apr 1989
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	Provides a known fatal-error exit for procedures (such as
 *		'doalloc()').  Some applications (such as 'ded') may require
 *		a more complex exit; in these 'failed()' should be defined in
 *		the main-program module.
 *
 *		Usually this is called after a unix-style error occurs; so
 *		we use 'perror()' to report the problem.
 */

#include "ptypes.h"

MODULE_ID("$Id: failed.c,v 12.3 1993/10/29 17:35:26 tom Exp $")

void	failed(
	_AR1(char *,	s))
	_DCL(char *,	s)
{
	FFLUSH(stdout);
	perror(s);
	exit(FAIL);
	/*NOTREACHED*/
}

#ifdef	TEST
_MAIN
{
	failed(argc > 1 ? argv[1] : argv[0]);
	/*NOTREACHED*/
}
#endif
