#ifndef	lint
static	char	Id[] = "$Id: vecalloc.c,v 9.0 1991/05/15 09:58:05 ste_cm Rel $";
#endif

/*
 * Title:	vecalloc.c (vector-allocator)
 * Author:	T.E.Dickey
 * Created:	01 Dec 1987
 * $Log: vecalloc.c,v $
 * Revision 9.0  1991/05/15 09:58:05  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:58:05  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  88/05/17  09:40:23  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  88/05/17  09:40:23  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/05/17  09:40:23  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/05/17  09:40:23  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/05/17  09:40:23  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/05/17  09:40:23  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/05/17  09:40:23  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.4  88/05/17  09:40:23  dickey
 *		sccs2rcs keywords
 *		
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
