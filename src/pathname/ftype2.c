#ifndef	lint
static	char	Id[] = "$Id: ftype2.c,v 9.0 1991/06/07 11:56:13 ste_cm Rel $";
#endif

/*
 * Title:	ftype2.c (file-type/suffix #2)
 * Author:	T.E.Dickey
 * Created:	13 Sep 1988
 * $Log: ftype2.c,v $
 * Revision 9.0  1991/06/07 11:56:13  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.2  91/06/07  11:56:13  dickey
 *		*** empty log message ***
 *		
 *		Revision 8.1  91/05/15  09:25:41  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  89/10/04  12:11:05  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
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
