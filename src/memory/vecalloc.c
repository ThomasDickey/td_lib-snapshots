/*
 * Title:	vecalloc.c (vector-allocator)
 * Author:	T.E.Dickey
 * Created:	01 Dec 1987
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
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

MODULE_ID("$Id: vecalloc.c,v 12.5 2004/03/07 22:03:45 tom Exp $")

char **
vecalloc(unsigned len)
{
    return (ALLOC(char *, len));
}

void
vecfree(char **s)
{
    dofree((char *) s);
}
