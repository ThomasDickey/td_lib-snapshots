#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: rcspath.c,v 12.2 1993/10/29 17:35:24 dickey Exp $";
#endif

/*
 * Title:	rcspath.c (rcs-utility to path-name)
 * Author:	T.E.Dickey
 * Created:	09 Sep 1988
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		04 Oct 1991, conversion to ANSI
 *		09 Sep 1991, lint
 *		20 Jun 1991, verify that utility exists, else failure
 *
 * Function:	This returns the pathname of the specified RCS-utility, and
 *		is intended to provide a measure of security in executing
 *		the utilities from a setuid program.
 */

#include	"ptypes.h"
#include	"rcsdefs.h"
#include	"rcspath.h"

char *	rcspath(
	_AR1(char *,	utility))
	_DCL(char *,	utility)
{
	static	char	bfr[BUFSIZ];
#ifdef	RCS_PATH
	auto	char	tmp[BUFSIZ];
	utility = pathcat(tmp, RCS_PATH, utility);
#endif
	if (which(bfr, sizeof(bfr), utility, ".") <= 0) {
		failed(utility);
		/*NOTREACHED*/
	}
	return bfr;
}
