#ifndef	lint
static	char	what[] = "$Id: failed.c,v 8.0 1989/08/17 09:05:22 ste_cm Rel $";
#endif	lint

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
#endif	TEST
