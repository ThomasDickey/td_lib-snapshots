#ifndef	lint
static	char	Id[] = "$Id: vecalloc.c,v 9.1 1991/09/13 07:37:54 dickey Exp $";
#endif

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

#ifdef	lint
#define	DOALLOC(c,n)	(c *)0
#else
#define	DOALLOC(c,n)	(c *)(doalloc((char *)0, sizeof(c) * (n)))
#endif

/*ARGSUSED*/
char **
vecalloc(len)
{
	return(DOALLOC(char *,len));
}

vecfree(s)
char	**s;
{
	dofree((char *)s);
}
