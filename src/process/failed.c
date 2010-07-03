/*
 * Title:	failed.c (generic exit from program)
 * Author:	T.E.Dickey
 * Created:	27 Apr 1989
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
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

MODULE_ID("$Id: failed.c,v 12.5 2010/07/03 16:11:51 tom Exp $")

void
failed(const char *s)
{
    FFLUSH(stdout);
    perror(s);
    exit(FAIL);
    /*NOTREACHED */
}

#ifdef	TEST
_MAIN
{
    failed(argc > 1 ? argv[1] : argv[0]);
    /*NOTREACHED */
}
#endif
