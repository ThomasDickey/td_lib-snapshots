/*
 * Title:	veclen.c (vector-length)
 * Author:	T.E.Dickey
 * Created:	16 Sep 1991
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	Computes the length of a list of vectors (e.g., pointers to
 *		strings) which is terminated by a null-pointer.
 *
 * Returns:	the length of the vector
 */

#include <ptypes.h>

MODULE_ID("$Id: veclen.c,v 12.4 2014/12/28 01:10:47 tom Exp $")

int
veclen(char **vector)
{
    if (vector != 0) {
	register int len;
	for (len = 0; vector[len] != 0; len++) ;
	return len;
    }
    return 0;
}

/******************************************************************************/
#ifdef	TEST
_MAIN
{
    (void) argc;
    (void) argv;
    exit(EXIT_FAILURE);
    /*NOTREACHED */
}
#endif /* TEST */
