#ifndef	lint
static	char	Id[] = "$Id: rcspath.c,v 9.1 1991/06/20 11:21:08 dickey Exp $";
#endif

/*
 * Title:	rcspath.c (rcs-utility to path-name)
 * Author:	T.E.Dickey
 * Created:	09 Sep 1988
 * Modified:
 *		20 Jun 1991, verify that utility exists, else failure
 *
 * Function:	This returns the pathname of the specified RCS-utility, and
 *		is intended to provide a measure of security in executing
 *		the utilities from a setuid program.
 */

#include	"ptypes.h"
#include	"rcspath.h"
extern	char	*pathcat();

char *
rcspath(utility)
char	*utility;
{
	static	char	bfr[BUFSIZ];
#ifdef	RCS_PATH
	auto	char	tmp[BUFSIZ];
	utility = pathcat(tmp, RCS_PATH, utility);
#endif
	if (which(bfr, sizeof(bfr), utility, ".") > 0)
		return bfr;
	failed(utility);
}
