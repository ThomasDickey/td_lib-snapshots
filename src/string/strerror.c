/*
 * Title:	strerror.c (unix compatibility)
 * Author:	T.E.Dickey
 * Created:	01 Jul 1994
 *
 * Function:	provide the unix 'strerror()' function if it isn't available
 */
#define	ERR_PTYPES
#define STR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: strerror.c,v 12.7 2002/07/05 11:18:55 tom Exp $")

#if !defined(HAVE_STRERROR)
char	*strerror (
	_AR1(int,	errnum))
	_DCL(int,	errnum)
{
	static	char	unknown[20] = "error ";
#if defined(HAVE_SYS_ERRLIST)
	if (errnum > 0 && errnum < sys_nerr)
		return sys_errlist[errnum];
#endif
	(void)l2str(unknown+6, (long)errnum, 10);
	return unknown;
}
#endif

#ifdef TEST
/*ARGSUSED*/
_MAIN
{
	int	n;
	for (n = -2; n < 10; n++)
		PRINTF("%d:%s\n", n, strerror(n));
	exit(EXIT_FAILURE);
}
#endif	/* TEST */
