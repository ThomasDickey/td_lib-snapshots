#ifndef	lint
static	char	Id[] = "$Id: stralloc.c,v 8.0 1989/10/04 12:53:52 ste_cm Rel $";
#endif	lint

/*
 * Title:	stralloc.c (string-allocator)
 * Author:	T.E.Dickey
 * Created:	01 Dec 1987
 * $Log: stralloc.c,v $
 * Revision 8.0  1989/10/04 12:53:52  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  89/10/04  12:53:52  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/10/04  12:53:52  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/10/04  12:53:52  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.1  89/10/04  12:53:52  dickey
 *		lint (apollo SR10.1)
 *		
 *		Revision 4.0  88/05/10  13:16:06  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/05/10  13:16:06  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/05/10  13:16:06  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.3  88/05/10  13:16:06  dickey
 *		sccs2rcs keywords
 *		
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

char *
stralloc(s)
char	*s;
{
	return(strcpy(doalloc((char *)0, (unsigned)strlen(s)+1), s));
}

strfree(s)
char	*s;
{
	dofree(s);
}
