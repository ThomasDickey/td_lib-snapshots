/*
 * Title:	sccspath.c (rcs-utility to path-name)
 * Author:	T.E.Dickey
 * Created:	13 Jul 1994, from 'rcspath.c'
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *
 * Function:	This returns the pathname of the specified SCCS-utility, and
 *		is intended to provide a measure of security in executing
 *		the utilities from a setuid program.
 */

#include	"ptypes.h"
#include	"sccsdefs.h"

MODULE_ID("$Id: sccspath.c,v 12.3 2004/03/07 16:31:58 tom Exp $")

char *
sccspath(char *utility)
{
    static char bfr[BUFSIZ];
#ifdef	SCCS_PATH
    char tmp[BUFSIZ];
    utility = pathcat(tmp, SCCS_PATH, utility);
#endif
    if (which(bfr, sizeof(bfr), utility, ".") <= 0) {
	failed(utility);
	/*NOTREACHED */
    }
    return bfr;
}
