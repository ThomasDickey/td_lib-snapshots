#ifndef	NO_IDENT
static	char	Id[] = "$Id: strerror.c,v 12.1 1994/07/01 23:49:55 tom Exp $";
#endif

/*
 * Title:	strerror.c (unix compatibility)
 * Author:	T.E.Dickey
 * Created:	01 Jul 1994
 *
 * Function:	provide the unix 'strerror()' function if it isn't available
 */
#include "ptypes"

#if !HAVE_STRERROR
char	*strerror (
	_AR1(int,	errnum))
	_DCL(int,	errnum)
{
	static	char	unknown[20] = "error ";
#if HAVE_SYS_ERRLIST && HAVE_SYS_NERR
	if (errnum > 0 && errnum < sys_nerr)
		return sys_errlist[errnum];
#endif
	(void)ltostr(unknown+6, (long)errnum, 10);
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
