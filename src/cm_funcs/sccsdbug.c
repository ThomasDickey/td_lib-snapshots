/*
 * Title:	sccs_debug.c (rcs debug-flag)
 * Author:	T.E.Dickey
 * Created:	30 May 1994
 * Modified:
 *
 * Function:	Encapsulates knowledge of the SCCS_DEBUG enviromnent variable
 *		so that it can be easily set/cleared from C-shell or Bourne
 *		shell.
 *
 *		We return true iff the variable is both defined and non-null.
 *		If the variable is a numeric constant, we return that value.
 */

#include "ptypes.h"
#include "sccsdefs.h"

MODULE_ID("$Id: sccsdbug.c,v 12.2 1994/05/30 18:34:59 tom Exp $")

int	sccs_debug(_AR0)
{
	static	int	flag = -1;
	register char	*s;

	if (flag < 0) {
		flag = 0;
		if ((s = getenv("SCCS_DEBUG")) != 0) {
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
