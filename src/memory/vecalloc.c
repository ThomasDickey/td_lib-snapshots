#ifndef	lint
static	char	Id[] = "$Id: vecalloc.c,v 12.0 1991/10/04 15:52:45 ste_cm Rel $";
#endif

/*
 * Title:	vecalloc.c (vector-allocator)
 * Author:	T.E.Dickey
 * Created:	01 Dec 1987
 * Modified:
 *		04 Oct 1991, conversion to ANSI
 *
 * Function:	Allocate space for a given vector (of pointers), and
 *		return a pointer to the non-volatile storage.  The
 *		complementary operation 'vecfree()' releases the
 *		stored vector.
 */

#include "ptypes.h"

	/*ARGSUSED*/
	def_ALLOC(char *);

char **
vecalloc(
_AR1(unsigned,	len))
_DCL(unsigned,	len)
{
	return(ALLOC(char *,len));
}

vecfree(
_AR1(char **,	s))
_DCL(char **,	s)
{
	dofree((char *)s);
}
