/*
 * Author:	T.E.Dickey
 * Title:	scr_size.c (obtain screen size)
 * Created:	27 Jul 1988
 * Modified:
 *		26 Jun 1994, use BSD-style ioctl to obtain screen size.
 *		29 Oct 1993, ifdef-ident. Compile on HP/UX.
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, convert to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		01 May 1991, when rlogin'd from vax/vms to apollo, 'tgetnum()'
 *			     does not work properly, even though 'tgetent()'
 *			     returns correct data.  Check for error-return from
 *			     'tgetnum()' to provide default 24 by 80 screen.
 *		23 May 1990, apollo sr10.2 (X11.3) returns an off-by-one error
 *			     for the 'termcap' results.
 *		12 Mar 1990, lint (apollo sr10.1)
 *		04 Oct 1989, modified (in SR10.1 conversion) so that the
 *			     TEST-program generated from this is the same as the
 *			     'scr_size' program I use in running 'rterm' to the
 *			     sun's
 *
 * Function:	inquire to see the size of VT100 window
 *
 * Returns:	A negative code if any error is found, 0 for normal (termcap)
 *		usage, and a positive number for system-dependent sizing (i.e.,
 *		an Apollo screen).
 */

#define	TRM_PTYPES
#include "ptypes.h"
#include "td_curse.h"

MODULE_ID("$Id: scr_size.c,v 12.6 1995/07/06 14:03:48 tom Exp $")

#ifdef	apollo
#  ifdef	apollo_sr10
#    define sr10_bug	1
#    include <apollo/base.h>
#  else
#    include </sys/ins/base.ins.c>
#  endif	/* apollo_sr10	*/
#else	/* unix */
#  if HAVE_IOCTL_H
#    include <ioctl.h>
#  else
#    if HAVE_SYS_IOCTL_H
#      include <sys/ioctl.h>
#    endif
#  endif
#endif	/* apollo/unix	*/

#ifndef sr10_bug
# define sr10_bug	0
#endif

#define	my_LINES	retval[0]
#define	my_COLS		retval[1]

int	scr_size(
	_AR1(int *,	retval))
	_DCL(int *,	retval)
{
#ifdef TIOCGWINSZ
	struct winsize size;
#endif
	char	i_blk[1024];

	/*
	 * Test for an Apollo screen first, so that we can distinguish between
	 * Apollo's vt100 emulator (return=1) and X-windows (return=0).  If we
	 * reversed this, the Apollo would always go from pad to vt100 and give
	 * a value.  Testing Apollo first lets us verify if we are in a vt100
	 * emulator running under the Display Manager.
	 */
#ifdef	apollo
	extern	void	vte_$inq_screen_size();
	stream_$id_t	id	= ios_$stdout;
	status_$t	st;

	struct	{
		short	height;
		short	width;
		} screen_size;

	vte_$inq_screen_size(&id, &screen_size, &st);
	if (st.all == status_$ok) {
		my_LINES = screen_size.height;
		my_COLS  = screen_size.width;
		return (1);
	}
#endif	/* apollo */

#ifdef TIOCGWINSZ	/* SunOS 4.x */
	if (ioctl (fileno(stdin), TIOCGWINSZ, (caddr_t)&size) == 0)
	{
		if ((int)(size.ws_row) > 0)
			my_LINES = size.ws_row;
		if ((int)(size.ws_col) > 0)
			my_COLS = size.ws_col;
		return (0);
	}
	else
#ifdef	__svr4__
	{
		/*
		 * Testing on ClarkNet's machine:
		 *	SunOS explorer 5.3 Generic_Patch sun4m sparc
		 * I found that if I tried to use the termcap functions, that
		 * they clobber the standard input used by curses.  So this
		 * error-return is designed to avoid the termcap code on
		 * Solaris until I can refine the bug -- T.Dickey 94/7/23.
		 */
		return (-1);
	}
#endif	/* __svr4__ */
#endif	/* TIOCGWINSZ */

	/*
	 * If we can get the size from termcap, let's believe it.
	 * patch: There doesn't seem to be any way we can get window-resizing
	 * info under X11.2 on Apollo SR9.7
	 */
	if (tgetent(i_blk,getenv("TERM")) >= 0)
	{
		if ((my_LINES = tgetnum("li")) < 0) my_LINES = 24 - sr10_bug;
		if ((my_COLS  = tgetnum("co")) < 0) my_COLS  = 80 - sr10_bug;
#if sr10_bug
		my_LINES += sr10_bug;
		my_COLS  += sr10_bug;
#endif
		return (0);
	}
	return (-1);
}

#ifdef	TEST
#include <stdio.h>
_MAIN
{
	int	lc[2];
	int	code;

	if ((code = scr_size(lc)) >= 0) {
		FPRINTF(stderr, "%s %d lines, %d columns\n",
			code ? "Termcap" : "Size:",
			lc[0], lc[1]);
	} else
		FPRINTF(stderr, "?? cannot determine screen size\n");
	if (code <= 0)
		sleep(3);	/* in case we were in a pad */
	exit(SUCCESS);
}
#endif
