#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: strcount.c,v 12.1 1993/10/29 17:35:23 dickey Exp $";
#endif

/*
 * Title:	strcount.c
 * Author:	T.E.Dickey
 * Created:	10 Feb 1992
 * Modified:
 *		29 Oct 1993, ifdef-ident
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
