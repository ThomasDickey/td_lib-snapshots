#ifndef	lint
static	char	Id[] = "$Id: rawterm.c,v 9.0 1991/05/15 10:04:29 ste_cm Rel $";
#endif

/*
 * Title:	rawterm.c (set terminal to raw mode)
 * Author:	T.E.Dickey
 * Created:	24 Nov 1987
 * $Log: rawterm.c,v $
 * Revision 9.0  1991/05/15 10:04:29  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  10:04:29  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  88/08/11  08:32:10  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  88/08/11  08:32:10  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/08/11  08:32:10  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/08/11  08:32:10  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/08/11  08:32:10  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/08/11  08:32:10  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/11  08:32:10  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.6  88/08/11  08:32:10  dickey
 *		sccs2rcs keywords
 *		
 *		03 Aug 1988, use 'crmode()' rather than 'raw()' so we can see
 *			     signals in caller.
 *		28 Jul 1988, added 'nl()' to items affected.
 *
 * Function:	Set terminal to single-character mode
 */

#define		CUR_PTYPES
#include	"ptypes.h"

/*
 * Set terminal to single-character mode
 */
rawterm()
{
#ifdef	SYSTEM5
	cbreak();
#else	/* SYSTEM5 */
	crmode();
#endif	/* SYSTEM5 */
	noecho();
	nonl();
}
