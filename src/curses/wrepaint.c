#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: wrepaint.c,v 12.2 1993/11/01 17:39:14 dickey Exp $";
#endif

/*
 * Title:	wrepaint.c (repaint-window)
 * Author:	T.E.Dickey
 * Created:	24 Aug 1989
 * Modified:
 *		01 Nov 1993, ported to HP/UX (SYSTEM5). This works well enough
 *			     to manage the curses output, but not the non-curses
 *			     output -- for that we must still clear the screen.
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

void	wrepaint(
	_ARX(WINDOW *,	win)
	_AR1(int,	row)
		)
	_DCL(WINDOW *,	win)
	_DCL(int,	row)
{
#ifdef	SYSTEM5
	WINDOW	*tmp;
	int	y, x;

	tmp = newwin(LINES-row, COLS, row, 0);
	getyx(win, y, x);

	overwrite(win,tmp);
	wmove(win, row, 0);
	wclrtobot(win);

	wrefresh(tmp);
	overwrite(tmp,win);
	delwin(tmp);
	wmove(win, y, x);
	wrefresh(win);
#else
	auto	int	min_row = win->_begy,
			min_col = win->_begx,
			max_col = win->_maxx + min_col - 1;
	register int	col, c;

	while (row < LINES) {
		chtype	*y_data = win->_y[row-min_row];
		win->_firstch[row] = 0;
		win->_lastch [row] = win->_maxx - 1;
		for (col = min_col; col < max_col; col++) {
			if ((c = y_data[col-min_col]) == '~')
				c = '?';
			else
				c = '~';
			curscr->_y[row][col] = c;
		}
		row++;
	}
	clearok(win, FALSE);
	(void)wrefresh(win);
#endif
}
