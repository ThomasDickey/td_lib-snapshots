/*
 * Title:	killchar.c (return kill-char value)
 * Author:	T.E.Dickey
 * Created:	24 Mar 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		01 Jan 2000, 'killchar()' is a macro in bsd4.4 curses
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		17 Aug 1988, gould defines this as a macro.
 *		11 Aug 1988, don't use on system5, since we want to use curses.
 */

#include	"td_curse.h"

MODULE_ID("$Id: killchar.c,v 12.9 2014/07/22 13:51:29 tom Exp $")

#if	!defined(HAVE_KILLCHAR) && !defined(killchar)
int
killchar(void)
{
    int code = '\177';
    struct sgttyb buf;

    if (ioctl(0, TIOCGETP, (caddr_t) & buf) >= 0)
	code = buf.sg_kill;
    return (code);
}
#else
void dummy_killchar(void);
void
dummy_killchar(void)
{
}
#endif /* HAVE_KILLCHAR */
