#ifndef	lint
static	char	Id[] = "$Id: rawterm.c,v 12.0 1991/10/03 15:00:23 ste_cm Rel $";
#endif

/*
 * Title:	rawterm.c (set terminal to raw mode)
 * Author:	T.E.Dickey
 * Created:	24 Nov 1987
 * Modified:
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
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
rawterm(_AR0)
{
#ifdef	SYSTEM5
	cbreak();
#else	/* SYSTEM5 */
	crmode();
#endif	/* SYSTEM5 */
	noecho();
	nonl();
}
