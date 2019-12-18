/*
 * Author:	T.E.Dickey
 * Title:	resizewin.c (change size of curses window)
 * Created:	21 Apr 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		03 Jul 2003, modify ifdef to use resizeterm() even if curses
 *			     is not apparently ncurses.
 *		15 Feb 1998, cannot use savewin on OSF1 4.0; chtype is struct
 *		03 Sep 1995, mods for bsd4.4 curses
 *		04 Jul 1995, integration with ncurses 1.9.3
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

MODULE_ID("$Id: resizwin.c,v 12.23 2019/12/17 23:38:56 tom Exp $")

#if defined(HAVE_RESIZETERM)
extern WINDOW *newscr;
#endif

#define	my_LINES	size[0]
#define	my_COLS		size[1]

int
resizewin(void)
{
    int lc[2];

    lc[0] = LINES;
    lc[1] = COLS;

    if (scr_size(lc) >= 0) {
	static int size[2];

	my_LINES = lc[0];
	my_COLS = lc[1];
	if (my_LINES != LINES || my_COLS != COLS) {
#if defined(CURSES_LIKE_BSD) && (defined(TYPE_CCHAR_T_IS_SCALAR) || !defined(HAVE_TYPE_CCHAR_T)) && (defined(TYPE_CHTYPE_IS_SCALAR) || !defined(HAVE_TYPE_CHTYPE))
	    wresize(stdscr, my_LINES, my_COLS);
	    wresize(curscr, my_LINES, my_COLS);
	    LINES = my_LINES;
	    COLS = my_COLS;
	    savewin();
	    unsavewin(TRUE, 0);
	    return (TRUE);
#endif
#if defined(CURSES_LIKE_BSD44)
	    wresize(stdscr, my_LINES, my_COLS);
	    wresize(curscr, my_LINES, my_COLS);
	    LINES = my_LINES;
	    COLS = my_COLS;
	    return (TRUE);
#endif
#if defined(CURSES_LIKE_SYSV)
#ifdef __hpux
	    dlog_comment("resizewin called\n");
	    dlog_comment("..., LINES %d, COLS %d\n", LINES, COLS);
	    dlog_comment("..., scr_size (%d, %d)\n", lc[0], lc[1]);
	    dlog_flush();
#endif
	    /*
	     * This might work
	     */
	    endwin();
	    refresh();
	    return (TRUE);
#endif
#if defined(HAVE_WRESIZE) && defined(HAVE_RESIZETERM)
	    resizeterm(my_LINES, my_COLS);
	    wrefresh(curscr);
	    return (TRUE);
#endif
	}
    }
    return (FALSE);
}

/******************************************************************************/
#ifdef	TEST
_MAIN
{
    (void) argc;
    (void) argv;
    exit(EXIT_FAILURE);
    /*NOTREACHED */
}
#endif /* TEST */
