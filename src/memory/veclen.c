#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: veclen.c,v 12.1 1993/10/29 17:35:23 dickey Exp $";
#endif

/*
 * Title:	veclen.c (vector-length)
 * Author:	T.E.Dickey
 * Created:	16 Sep 1991
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	Computes the length of a list of vectors (e.g., pointers to
 *		strings) which is terminated by a null-pointer.
 *
 * Returns:	the length of the vector
 */

#include <ptypes.h>

int
veclen(
_AR1(char **,	vector)
	)
_DCL(char **,	vector)
{
	if (vector != 0) {
		register int	len;
		for (len = 0; vector[len] != 0; len++);
		return len;
	}
	return 0;
}
