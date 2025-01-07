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

MODULE_ID("$Id: veclen.c,v 12.5 2025/01/07 00:29:46 tom Exp $")

int
veclen(char **vector)
{
    if (vector != NULL) {
	register int len;
	for (len = 0; vector[len] != NULL; len++) ;
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
