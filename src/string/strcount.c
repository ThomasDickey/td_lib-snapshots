#ifndef	lint
static	char	*Id = "$Id: strcount.c,v 12.0 1992/02/10 07:46:49 ste_cm Rel $";
#endif

/*
 * Title:	strcount.c
 * Author:	T.E.Dickey
 * Created:	10 Feb 1992
 *
 * Function:	counts occurrences of a given character within a string.
 */

#include "ptypes.h"

int
strcount(
_ARX(register char *,	s)
_AR1(register int,	c)
	)
_DCL(register char *,	s)
_DCL(register int,	c)
{
	register int	count = 0;
	if (s != 0)
		while (*s != EOS)
			if (*s++ == c)
				count++;
	return count;
}
