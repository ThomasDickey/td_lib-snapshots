/*
 * Title:	wrepaint.c (repaint-window)
 * Author:	T.E.Dickey
 * Created:	24 Aug 1989
 * Modified:
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

MODULE_ID("$Id: wrepaint.c,v 12.14 2002/07/05 11:18:55 tom Exp $")

void	wrepaint(
	_ARX(WINDOW *,	win)
	_AR1(int,	row)
		)
	_DCL(WINDOW *,	win)
	_DCL(int,	row)
{
	/* SysV curses has a workable 'touchwin()'; the older BSD curses
	 * doesn't.  We test indirectly for this by looking for 'keypad()',
	 * which generally appears only in combination with a working
	 * 'touchwin()'.
	 */
#if	defined(GOOD_TOUCHWIN)
	touchwin(win);
	wrefresh(curscr);
#else
	/*
	 * Otherwise, try to work around.  This works with BSD 4.2 and BSD 4.3;
	 * but not however, with BSD 4.4, which has a partially functional
	 * touchwin.
	 */
	auto	int	min_row = wBegY(win),
			min_col = wBegX(win),
			max_col = wMaxX(win) + min_col - 1;
	register int	col, c;

	while (row < LINES) {
		CursesFirstCh(win,row) = 0;
		CursesLastCh(win,row)  = wMaxX(win) - 1;
		for (col = min_col; col < max_col; col++) {
			if ((c = CursesData(win,row-min_row,col-min_col)) == '~')
				c = '?';
			else
				c = '~';
			CursesData(curscr,row,col) = c;
		}
		row++;
	}
	clearok(win, FALSE);
#if defined(HAVE_TOUCHWIN)
	touchwin(win); /* this does no harm, and makes BSD 4.4 work */
#endif
	(void)wrefresh(win);
#endif
}
