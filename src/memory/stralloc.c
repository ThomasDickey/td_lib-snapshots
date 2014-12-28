/*
 * Title:	stralloc.c (string-allocator)
 * Author:	T.E.Dickey
 * Created:	01 Dec 1987
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		04 Oct 1989, lint (apollo SR10.1)
 *		10 May 1988, route 'free' via 'dofree()'.
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

#define	STR_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: stralloc.c,v 12.7 2014/12/28 01:10:47 tom Exp $")

char *
stralloc(const char *s)
{
    return (strcpy(doalloc((char *) 0, strlen(s) + 1), s));
}

void
strfree(char *s)
{
    dofree(s);
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
