#ifndef	lint
static	char	sccs_id[] = "@(#)erasechar.c	1.3 88/08/12 10:04:26";
#endif	lint

/*
 * Title:	erasechar.c (return erase-char value)
 * Author:	T.E.Dickey
 * Created:	24 Mar 1988
 * Modified:
 *		11 Aug 1988, don't use on system5, since we want to use curses.
 */

#include "ptypes.h"
#include <sgtty.h>

#ifndef	SYSTEM5
char
erasechar()
{
int	code	= '\b';
struct	sgttyb	buf;

	if (ioctl(0, TIOCGETP, &buf) >= 0)
		code = buf.sg_erase;
	return (code);
}
#endif	SYSTEM5
