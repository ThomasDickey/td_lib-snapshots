#ifndef	lint
static	char	Id[] = "$Id: ftype.c,v 9.0 1991/06/07 11:55:25 ste_cm Rel $";
#endif

/*
 * Title:	ftype.c (file-type/suffix)
 * Author:	T.E.Dickey
 * Created:	13 Sep 1988
 * $Log: ftype.c,v $
 * Revision 9.0  1991/06/07 11:55:25  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.2  91/06/07  11:55:25  dickey
 *		*** empty log message ***
 *		
 *		Revision 8.1  91/05/15  10:17:46  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  89/10/04  11:52:08  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 * Function:	returns a pointer to the left-most "."-suffix of the leaf of
 *		the given pathname.
 */

#define	STR_PTYPES
#include	"ptypes.h"

#ifdef	vms
#define	PATH_END	']'
#else	/* unix */
#define	PATH_END	'/'
#endif	/* vms/unix */

char *
ftype(path)
char *path;
{
	register char	*s,
			*t;

	if ((s = strrchr(path, PATH_END)) == 0)	/* find leaf */
		s = path;
	else	
		s++;
	if ((t = strchr(s, '.')) == 0)		/* ...and suffix in leaf */
		t = s + strlen(s);
	return (t);
}
