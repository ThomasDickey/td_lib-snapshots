/*
 * Title:	rcs_debug.c (rcs debug-flag)
 * Author:	T.E.Dickey
 * Created:	15 Mar 1989
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		04 Oct 1991, conversion to ANSI
 *		06 Sep 1991, return a value, allowing debug-levels
 *
 * Function:	Encapsulates knowledge of the RCS_DEBUG enviromnent variable
 *		so that it can be easily set/cleared from C-shell or Bourne
 *		shell.
 *
 *		We return true iff the variable is both defined and non-null.
 *		If the variable is a numeric constant, we return that value.
 */

#include "ptypes.h"
#include "rcsdefs.h"

MODULE_ID("$Id: rcsdebug.c,v 12.3 1993/10/29 17:35:25 tom Exp $")

int	rcs_debug(_AR0)
{
	static	int	flag = -1;
	register char	*s;

	if (flag < 0) {
		flag = 0;
		if ((s = getenv("RCS_DEBUG")) != 0) {
			if (*s) {
				char	*d;
				flag = strtol(s, &d, 0);
				if (*d)
					flag = 1;	/* not a number */
			}
		}
	}
	return (flag);
}
