#ifndef	lint
static	char	Id[] = "$Id: erasechr.c,v 11.0 1991/10/03 16:04:43 ste_cm Rel $";
#endif

/*
 * Title:	erasechar.c (return erase-char value)
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

#ifndef	erasechar
#ifndef	SYSTEM5
char
erasechar(_AR0)
{
int	code	= '\b';
struct	sgttyb	buf;

	if (ioctl(0, TIOCGETP, &buf) >= 0)
		code = buf.sg_erase;
	return (code);
}
#endif
#endif
