#ifndef	lint
static	char	sccs_id[] = "@(#)killchar.c	1.1 88/03/24 14:25:33";
#endif	lint

/*
 * Title:	killchar.c (return kill-char value)
 * Author:	T.E.Dickey
 * Created:	24 Mar 1988
 * Modified:
 *
 */

#include <sgtty.h>

killchar()
{
int	code	= '\177';
struct	sgttyb	buf;

	if (ioctl(0, TIOCGETP, &buf) >= 0)
		code = buf.sg_kill;
	return (code);
}
