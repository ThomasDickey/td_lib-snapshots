/*
 * Title:	erasechar.c (return erase-char value)
 * Author:	T.E.Dickey
 * Created:	24 Mar 1988
 * Modified:
 *		01 Jan 2000, 'erasechar()' is a macro in bsd4.4 curses
 *		04 Jul 1994, autoconf mods.
 *		19 May 1994, port to Solaris
 *		26 Apr 1994, port to Linux
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		05 Aug 1992, added 'eraseword()'
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		17 Aug 1988, gould defines this as a macro.
 *		11 Aug 1988, don't use on system5, since we want to use curses.
 */

#define  TRM_PTYPES
#include "td_curse.h"

MODULE_ID("$Id: erasechr.c,v 12.11 2002/07/05 11:22:17 tom Exp $")

#define	STDIN_FD 0

#if	!defined(HAVE_ERASECHAR) && !defined(erasechar)
int	erasechar(_AR0)
{
	int	code	= '\b';		/* default value */
	TermioT buf;
#if defined(USING_TERMIOS_H)
	if (tcgetattr(0, &buf) >= 0)
		code = buf.c_cc[VERASE];
#else
# if defined(USING_TERMIO_H)
	if (ioctl(0, TCGETA, (char *)&buf) >= 0)
		code = buf.c_cc[VERASE];
# else
#  if defined(USING_SGTTY_H)
	if (ioctl(STDIN_FD, TIOCGETP, (caddr_t)&buf) >= 0)
		code = buf.sg_erase;
#  endif
# endif
#endif
	return (code);
}
#endif	/* !HAVE_ERASECHAR */

int	eraseword(_AR0)
{
	int	code	= CTL('W');	/* default value */
#if defined(USING_TERMIOS_H)
	TermioT buf;
# ifdef VWERASE				/* SunOS has it */
	if (tcgetattr(0, &buf) >= 0)
		code = buf.c_cc[VWERASE];
# endif
#else
# if defined(USING_TERMIO_H)
	if (ioctl(0, TCGETA, (char *)&buf) >= 0)
		code = buf.c_cc[VWERASE];
# else
#  if defined(USING_SGTTY_H)
	struct ltchars buf;

	if (ioctl(STDIN_FD, TIOCGLTC, (caddr_t)&buf) >= 0)
		code = buf.t_werasc;
#  endif
# endif
#endif
	return (code);
}
