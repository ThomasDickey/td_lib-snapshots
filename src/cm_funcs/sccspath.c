/*
 * Title:	sccspath.c (rcs-utility to path-name)
 * Author:	T.E.Dickey
 * Created:	13 Jul 1994, from 'rcspath.c'
 * Modified:
 *
 * Function:	This returns the pathname of the specified SCCS-utility, and
 *		is intended to provide a measure of security in executing
 *		the utilities from a setuid program.
 */

#include	"ptypes.h"
#include	"sccsdefs.h"

MODULE_ID("$Id: sccspath.c,v 12.2 1994/07/14 01:34:57 tom Exp $")

char *	sccspath(
	_AR1(char *,	utility))
	_DCL(char *,	utility)
{
	static	char	bfr[BUFSIZ];
#ifdef	SCCS_PATH
	auto	char	tmp[BUFSIZ];
	utility = pathcat(tmp, SCCS_PATH, utility);
#endif
	if (which(bfr, sizeof(bfr), utility, ".") <= 0) {
		failed(utility);
		/*NOTREACHED*/
	}
	return bfr;
}
