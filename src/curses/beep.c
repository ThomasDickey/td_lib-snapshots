#ifndef	lint
static	char	Id[] = "$Id: beep.c,v 12.1 1993/09/21 18:54:05 dickey Exp $";
#endif

/*
 * Title:	beep.c (sound audible alarm)
 * Author:	T.E.Dickey
 * Created:	24 Mar (from 'ded.c')
 * Modified:
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		11 Aug 1988, since we use this only for curses applications,
 *			     don't use it in system5!
 */

#include	"td_curse.h"

#ifndef	SYSTEM5
void	beep(_AR0)
{
	blip('\007');
}
#endif	/* SYSTEM5 */
