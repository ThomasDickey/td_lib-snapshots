#ifndef	lint
static	char	Id[] = "$Id: killchar.c,v 12.0 1991/10/03 16:03:42 ste_cm Rel $";
#endif

/*
 * Title:	killchar.c (return kill-char value)
 * Author:	T.E.Dickey
 * Created:	24 Mar 1988
 * Modified:
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		17 Aug 1988, gould defines this as a macro.
 *		11 Aug 1988, don't use on system5, since we want to use curses.
 */

#define		CUR_PTYPES
#include	"ptypes.h"
#include	<sgtty.h>

#ifndef	killchar
#ifndef	SYSTEM5
char
killchar(_AR0)
{
int	code	= '\177';
struct	sgttyb	buf;

	if (ioctl(0, TIOCGETP, &buf) >= 0)
		code = buf.sg_kill;
	return (code);
}
#endif
#endif
