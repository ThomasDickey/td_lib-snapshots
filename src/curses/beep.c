#ifndef	lint
static	char	Id[] = "$Id: beep.c,v 11.0 1991/10/03 16:16:16 ste_cm Rel $";
#endif

/*
 * Title:	beep.c (sound audible alarm)
 * Author:	T.E.Dickey
 * Created:	24 Mar (from 'ded.c')
 * Modified:
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		11 Aug 1988, since we use this only for curses applications,
 *			     don't use it in system5!
 */

#include	"ptypes.h"

#ifndef	SYSTEM5
beep(_AR0)
{
	blip('\007');
}
#endif	/* SYSTEM5 */
