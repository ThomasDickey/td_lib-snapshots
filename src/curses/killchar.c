#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: killchar.c,v 12.3 1993/11/26 22:03:56 dickey Exp $";
#endif

/*
 * Title:	killchar.c (return kill-char value)
 * Author:	T.E.Dickey
 * Created:	24 Mar 1988
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		17 Aug 1988, gould defines this as a macro.
 *		11 Aug 1988, don't use on system5, since we want to use curses.
 */

#include	"td_curse.h"

#ifndef	killchar
#if	!SYS5_CURSES
int	killchar(_AR0)
{
	int	code	= '\177';
	struct	sgttyb	buf;

	if (ioctl(0, TIOCGETP, (caddr_t)&buf) >= 0)
		code = buf.sg_kill;
	return (code);
}
#endif	/* SYS5_CURSES */
#endif	/* killchar */
