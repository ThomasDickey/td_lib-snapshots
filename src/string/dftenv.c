/*
 * Title:	dftenv.c (getenv with default)
 * Author:	T.E.Dickey
 * Created:	08 Aug 1989
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	Performs a 'getenv()' and returns the environment value if it
 *		is defined, otherwise the value supplied by the user as an arg.
 */

#include "ptypes.h"

MODULE_ID("$Id: dftenv.c,v 12.3 2004/03/07 22:03:45 tom Exp $")

char *
dftenv(char *value, char *name)
{
    char *s;
    if ((s = getenv(name)) == 0)
	s = value;
    return (s);
}
