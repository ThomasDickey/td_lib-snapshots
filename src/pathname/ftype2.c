#ifndef	lint
static	char	Id[] = "$Id: ftype2.c,v 8.0 1989/10/04 12:11:05 ste_cm Rel $";
#endif	lint

/*
 * Title:	ftype2.c (file-type/suffix #2)
 * Author:	T.E.Dickey
 * Created:	13 Sep 1988
 * $Log: ftype2.c,v $
 * Revision 8.0  1989/10/04 12:11:05  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  89/10/04  12:11:05  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/10/04  12:11:05  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/10/04  12:11:05  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.1  89/10/04  12:11:05  dickey
 *		lint (apollo SR10.1)
 *		
 *		Revision 4.0  88/09/13  06:46:55  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/09/13  06:46:55  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/09/13  06:46:55  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  88/09/13  06:46:55  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	returns a pointer to the right-most "."-suffix of the leaf of
 *		the given pathname.
 */

#define	STR_PTYPES
#include	"ptypes.h"

char *
ftype2(path)
char *path;
{
	register char	*s,
			*t;

	if ((s = strrchr(path, '/')) == 0)	/* find leaf */
		s = path;
	else	
		s++;
	if ((t = strrchr(s, '.')) == 0)		/* ...and suffix in leaf */
		t = s + strlen(s);
	return (t);
}
