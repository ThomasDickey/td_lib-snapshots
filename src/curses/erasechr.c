#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: erasechr.c,v 12.3 1993/11/26 14:12:44 dickey Exp $";
#endif

/*
 * Title:	erasechar.c (return erase-char value)
 * Author:	T.E.Dickey
 * Created:	24 Mar 1988
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		05 Aug 1992, added 'eraseword()'
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		17 Aug 1988, gould defines this as a macro.
 *		11 Aug 1988, don't use on system5, since we want to use curses.
 */

#include	"td_curse.h"

#ifndef	erasechar
#if !SYS5_CURSES
int	erasechar(_AR0)
{
	int	code	= '\b';
	struct	sgttyb	buf;

	if (ioctl(0, TIOCGETP, (caddr_t)&buf) >= 0)
		code = buf.sg_erase;
	return (code);
}
#endif
#endif

int	eraseword(_AR0)
{
#if defined(__hpux) || defined(MSDOS)
	return -1;
#else
	int	code	= EOS;
	struct ltchars buf;

	if (ioctl(0, TIOCGLTC, (caddr_t)&buf) >= 0)
		code = buf.t_werasc;
	return (code);
#endif
}
