#ifndef	lint
static	char	sccs_id[] = "@(#)vecalloc.c	1.2 88/05/10 13:15:56";
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

char **
vecalloc(len)
{
	return((char **)(doalloc((char *)0, sizeof(char **) * len)));
}

vecfree(s)
char	**s;
{
	dofree((char *)s);
}
