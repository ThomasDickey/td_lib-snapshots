#ifndef	lint
static	char	sccs_id[] = "@(#)vecalloc.c	1.1 87/12/01 07:47:39";
#endif	lint

/*
 * Title:	vecalloc.c (vector-allocator)
 * Author:	T.E.Dickey
 * Created:	01 Dec 1987
 * Modified:
 *
 * Function:	Allocate space for a given vector (of pointers), and
 *		return a pointer to the non-volatile storage.  The
 *		complementary operation 'vecfree()' releases the
 *		stored vector.
 */

extern	char	*doalloc();
extern		free();

char **
vecalloc(len)
{
	return((char **)(doalloc((char *)0, sizeof(char **) * len)));
}

vecfree(s)
char	**s;
{
	free((char *)s);
}
