/*
 * Title:	beep.c (sound audible alarm)
 * Author:	T.E.Dickey
 * Created:	24 Mar (from 'ded.c')
 * Modified:
 *		26 Apr 1994, port to Linux
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		11 Aug 1988, since we use this only for curses applications,
 *			     don't use it in system5!
 */

#include	"td_curse.h"

MODULE_ID("$Id: beep.c,v 12.7 1994/05/23 22:24:59 tom Exp $")

#if !HAVE_BEEP
void	beep(_AR0)
{
	blip('\007');
}
#endif
