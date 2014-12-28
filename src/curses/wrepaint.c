/*
 * Title:	wrepaint.c (repaint-window)
 * Author:	T.E.Dickey
 * Created:	24 Aug 1989
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		20 Oct 1996, rework bsd4.4 compatibility with macros.
 *		04 Sep 1995, make this work with bsd4.4 curses
 *		26 Apr 1994, port to Linux
 *		01 Nov 1993, ported to HP/UX. This works well enough to manage
 *			     the curses output, but not the non-curses output
 *			     -- for that we must still clear the screen.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	Given a starting row-number, forces the screen to be redrawn
 *		without clearing it.
 *
 * Arguments:	win	=> window to repaint
 *		row	starting row number.
 *
 * Returns:	nothing.
 */

#include	"td_curse.h"

MODULE_ID("$Id: wrepaint.c,v 12.17 2014/12/28 01:10:44 tom Exp $")

void
wrepaint(WINDOW *win,
	 int row)
{
    /* SysV curses has a workable 'touchwin()'; the older BSD curses
     * doesn't.  We test indirectly for this by looking for 'keypad()',
     * which generally appears only in combination with a working
     * 'touchwin()'.
     */
#if	defined(GOOD_TOUCHWIN)
    (void) row;
    touchwin(win);
    wrefresh(curscr);
#else
    /*
     * Otherwise, try to work around.  This works with BSD 4.2 and BSD 4.3;
     * but not however, with BSD 4.4, which has a partially functional
     * touchwin.
     */
    int min_row = wBegY(win);
    int min_col = wBegX(win);
    int max_col = wMaxX(win) + min_col - 1;
    int col, c;

    while (row < LINES) {
	CursesFirstCh(win, row) = 0;
	CursesLastCh(win, row) = wMaxX(win) - 1;
	for (col = min_col; col < max_col; col++) {
	    if ((c = CursesData(win, row - min_row, col - min_col)) == '~')
		c = '?';
	    else
		c = '~';
	    CursesData(curscr, row, col) = c;
	}
	row++;
    }
    clearok(win, FALSE);
#if defined(HAVE_TOUCHWIN)
    touchwin(win);		/* this does no harm, and makes BSD 4.4 work */
#endif
    (void) wrefresh(win);
#endif
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
