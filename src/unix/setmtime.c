#ifndef	lint
static	char	Id[] = "$Id: setmtime.c,v 9.0 1991/05/15 09:54:24 ste_cm Rel $";
#endif

/*
 * Title:	setmtime.c (set file's mtime)
 * Author:	T.E.Dickey
 * Created:	20 May 1988
 * $Log: setmtime.c,v $
 * Revision 9.0  1991/05/15 09:54:24  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:54:24  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  88/05/20  12:49:49  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  88/05/20  12:49:49  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/05/20  12:49:49  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/05/20  12:49:49  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/05/20  12:49:49  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/05/20  12:49:49  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/05/20  12:49:49  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  88/05/20  12:49:49  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Set a file's mtime from a given reference value.  This is used
 *		in applications such as archival-extraction where the underlying
 *		tool/system defaults to the current date.
 */

#include	"ptypes.h"
extern	time_t	time();

setmtime(name, mtime)
char	*name;			/* name of file to touch */
time_t	mtime;			/* time we want to leave */
{
#ifdef	SYSTEM5
struct { time_t x, y; } tp;
	tp.x = time((time_t *)0);
	tp.y = mtime;
	return (utime(name, &tp));
#else
time_t	tv[2];
	tv[0] = time((time_t *)0);
	tv[1] = mtime;
	return (utime(name, tv));
#endif
}
