#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: resizwin.c,v 12.7 1995/02/11 19:21:04 tom Exp $";
#endif

/*
 * Author:	T.E.Dickey
 * Title:	resizewin.c (change size of curses window)
 * Created:	21 Apr 1988
 * Modified:
 *		23 Jul 1994, adaptations for ncurses and HP/UX curses.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		27 Jul 1988, broke out 'scr_size()' so we can use it to control
 *			     'padedit()'.
 *		13 May 1988, oops: was using old COLS, not new my_COLS to
 *			     reallocate row-strings.
 *		11 May 1988, reallocate firstch/lastch arrays (did not know what
 *			     they were til looking at curses source).
 *
 * Function:	Inquire to see if the terminal's screen has changed size since
 *		curses was initialized.  If so, adjust stdscr and curscr to
 *		match.
 *
 * Returns:	TRUE if a change has been made.
 *
 * Notes:
 *		This scheme works well with BSD curses, which has its state
 *		clearly defined in stdscr/curscr.  It doesn't always work well
 *		for SYS5 curses, which may have hidden state (e.g., ncurses has
 *		'newscr' and cached copies of lines/columns values).
 *
 *		If we (or curses) don't reset the state, the application will
 *		either crash or not repaint the screen properly.
 */

#include	"td_curse.h"

#if HAVE_LIBNCURSES
extern	WINDOW	*newscr;
#endif

#define	my_LINES	size[0]
#define	my_COLS		size[1]

int	resizewin(_AR0)
{
	static	int	size[2];
	auto	int	lc[2];

#ifdef __hpux
	lc[0] = LINES;
	lc[1] = COLS;
	if (scr_size(lc) >= 0) {
#ifdef TEST_RESIZE
		dlog_comment("resizewin called\n");
		dlog_comment("..., LINES %d, COLS %d\n", LINES, COLS);
		dlog_comment("..., scr_size (%d, %d)\n", lc[0], lc[1]);
		dlog_flush();
#else
		my_LINES = lc[0];
		my_COLS  = lc[1];
		if (my_LINES != LINES || my_COLS != COLS) {
			/* FIXME: if HP/UX curses does the resizing already,
			 * how do we detect it?
			 */
		dlog_comment("resizewin returns true\n");
			return TRUE;
		}
#endif
	}
	dlog_comment("resizewin returns false\n");
	return	FALSE;
#else	/* !__hpux */

	lc[0] = LINES;
	lc[1] = COLS;
	if (scr_size(lc) >= 0) {
		my_LINES = lc[0];
		my_COLS  = lc[1];
		if (my_LINES != LINES || my_COLS != COLS) {
#if SYS5_CURSES
#if HAVE_LIBNCURSES
			wresize(stdscr, my_LINES, my_COLS);
			wresize(curscr, my_LINES, my_COLS);
			wresize(newscr, my_LINES, my_COLS);
			resizeterm(my_LINES, my_COLS);
			savewin();
			unsavewin(TRUE,0);
			return (TRUE);
#else
			return (FALSE);	/* probably doesn't work */
#endif
#else	/* BSD curses */
			wresize(stdscr, my_LINES, my_COLS);
			wresize(curscr, my_LINES, my_COLS);
			LINES = my_LINES;
			COLS  = my_COLS;
			savewin();
			unsavewin(TRUE,0);
			return (TRUE);
#endif
		}
	}
	return (FALSE);
#endif	/* __hpux/!__hpux */
}
