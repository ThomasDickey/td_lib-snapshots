/*
 * Title:	dftenv.c (getenv with default)
 * Author:	T.E.Dickey
 * Created:	08 Aug 1989
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	Performs a 'getenv()' and returns the environment value if it
 *		is defined, otherwise the value supplied by the user as an arg.
 */

#include "ptypes.h"

MODULE_ID("$Id: dftenv.c,v 12.2 1993/10/29 17:35:27 tom Exp $")

char *
dftenv(
_ARX(char *,	value)
_AR1(char *,	name)
	)
_DCL(char *,	value)
_DCL(char *,	name)
{
	register char	*s;
	if ((s = getenv(name)) == 0)
		s = value;
	return (s);
}
