#ifndef	lint
static	char	sccs_id[] = "@(#)stralloc.c	1.1 87/12/01 08:54:23";
#endif	lint

/*
 * Title:	stralloc.c (string-allocator)
 * Author:	T.E.Dickey
 * Created:	01 Dec 1987
 * Modified:
 *
 * Function:	Allocate space for a given string, copy it to the
 *		allocated space, and return a pointer to the non-volatile
 *		string.  The complementary operation 'strfree()' releases
 *		the stored string.
 *
 * patch:	Should make this module handle storage allocation
 *		so that successive 'stralloc()' calls with the same
 *		string do not allocate new storage, but simply return
 *		a pointer to the old copy.
 *
 *		For now, assume that this operates with a linked list
 *		of strings, and reference-count decremented by 'strfree()'.
 */

extern	char	*doalloc();
extern	char	*strcpy();
extern		free();

char *
stralloc(s)
char	*s;
{
	return(strcpy(doalloc((char *)0, (unsigned)strlen(s)+1), s));
}

strfree(s)
char	*s;
{
	free(s);
}
