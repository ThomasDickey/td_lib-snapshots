#ifndef	lint
static	char	Id[] = "$Id: rcsdebug.c,v 9.0 1991/05/15 10:04:37 ste_cm Rel $";
#endif

/*
 * Title:	rcs_debug.c (rcs debug-flag)
 * Author:	T.E.Dickey
 * Created:	15 Mar 1989
 * $Log: rcsdebug.c,v $
 * Revision 9.0  1991/05/15 10:04:37  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  10:04:37  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  89/03/15  13:40:04  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  89/03/15  13:40:04  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/03/15  13:40:04  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/03/15  13:40:04  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  89/03/15  13:40:04  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  89/03/15  13:40:04  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  89/03/15  13:40:04  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  89/03/15  13:40:04  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Encapsulates knowledge of the RCS_DEBUG enviromnent variable
 *		so that it can be easily set/cleared from C-shell or Bourne
 *		shell.
 *
 *		We return true iff the variable is both defined and non-null.
 */

extern	char	*getenv();

rcs_debug()
{
	static	int	flag = -1;
	register char	*s;

	if (flag < 0) {
		flag = 0;
		if ((s = getenv("RCS_DEBUG")) != 0) {
			flag = (*s != '\0');
		}
	}
	return (flag);
}
