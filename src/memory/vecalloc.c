/*
 * Title:	vecalloc.c (vector-allocator)
 * Author:	T.E.Dickey
 * Created:	01 Dec 1987
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		04 Oct 1991, conversion to ANSI
 *
 * Function:	Allocate space for a given vector (of pointers), and
 *		return a pointer to the non-volatile storage.  The
 *		complementary operation 'vecfree()' releases the
 *		stored vector.
 */

#include "ptypes.h"

MODULE_ID("$Id: vecalloc.c,v 12.4 1994/11/17 13:23:17 tom Exp $")

	/*ARGSUSED*/
	def_ALLOC(char *)

char **	vecalloc(
	_AR1(unsigned,	len))
	_DCL(unsigned,	len)
{
	return(ALLOC(char *,len));
}

void	vecfree(
	_AR1(char **,	s))
	_DCL(char **,	s)
{
	dofree((char *)s);
}
