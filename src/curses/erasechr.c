#ifndef	lint
static	char	sccs_id[] = "@(#)erasechar.c	1.1 88/03/24 14:25:28";
#endif	lint

/*
 * Title:	erasechar.c (return erase-char value)
 * Author:	T.E.Dickey
 * Created:	24 Mar 1988
 * Modified:
 *
 */

#include <sgtty.h>

erasechar()
{
int	code	= '\b';
struct	sgttyb	buf;

	if (ioctl(0, TIOCGETP, &buf) >= 0)
		code = buf.sg_erase;
	return (code);
}
