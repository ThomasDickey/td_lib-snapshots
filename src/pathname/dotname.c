#ifndef	lint
static	char	Id[] = "$Id: dotname.c,v 9.0 1991/05/15 10:01:46 ste_cm Rel $";
#endif

/*
 * Created:	10 Dec 1985
 * $Log: dotname.c,v $
 * Revision 9.0  1991/05/15 10:01:46  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  10:01:46  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  85/12/16  09:28:47  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  85/12/16  09:28:47  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  85/12/16  09:28:47  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  85/12/16  09:28:47  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  85/12/16  09:28:47  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  85/12/16  09:28:47  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  85/12/16  09:28:47  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  85/12/16  09:28:47  dickey
 *		sccs2rcs keywords
 *		
 *		16 Dec 1985, oops: wrong return on "../n" forms.
 */
int	dotname (path)
register char	*path;
{
	if (*path++ == '.') {
		if (*path == '.')	path++;
		return (*path == '\0');
	} else				return (0);
}
