#ifndef	lint
static	char	Id[] = "$Id: failed.c,v 9.0 1991/05/15 10:03:35 ste_cm Rel $";
#endif

/*
 * Title:	failed.c (generic exit from program)
 * Author:	T.E.Dickey
 * Created:	27 Apr 1989
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

failed(s)
char	*s;
{
	perror(s);
	(void)fflush(stdout);
	(void)fflush(stderr);
	exit(FAIL);
	/*NOTREACHED*/
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
	failed(argc > 1 ? argv[1] : argv[0]);
	/*NOTREACHED*/
}
#endif
