#ifndef	lint
static	char	sccs_id[] = "@(#)killchar.c	1.3 88/08/12 10:04:51";
#endif	lint

/*
 * Title:	killchar.c (return kill-char value)
 * Author:	T.E.Dickey
 * Created:	24 Mar 1988
 * Modified:
 *		11 Aug 1988, don't use on system5, since we want to use curses.
 */

#include "ptypes.h"
#include <sgtty.h>

#ifndef	SYSTEM5
char
killchar()
{
int	code	= '\177';
struct	sgttyb	buf;

	if (ioctl(0, TIOCGETP, &buf) >= 0)
		code = buf.sg_kill;
	return (code);
}
#endif	SYSTEM5
