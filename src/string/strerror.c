/*
 * Title:	strerror.c (unix compatibility)
 * Author:	T.E.Dickey
 * Created:	01 Jul 1994
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *
 * Function:	provide the unix 'strerror()' function if it isn't available
 */
#define	ERR_PTYPES
#define STR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: strerror.c,v 12.8 2004/03/07 22:03:45 tom Exp $")

#if !defined(HAVE_STRERROR)
char *
strerror(int errnum)
{
    static char unknown[20] = "error ";
#if defined(HAVE_SYS_ERRLIST)
    if (errnum > 0 && errnum < sys_nerr)
	return sys_errlist[errnum];
#endif
    (void) l2str(unknown + 6, (long) errnum, 10);
    return unknown;
}
#endif

#ifdef TEST
/*ARGSUSED*/
_MAIN
{
    int n;
    for (n = -2; n < 10; n++)
	PRINTF("%d:%s\n", n, strerror(n));
    exit(EXIT_FAILURE);
}
#endif /* TEST */
