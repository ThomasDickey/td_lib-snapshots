#ifndef	lint
static	char	Id[] = "$Id: wrepaint.c,v 12.1 1993/09/21 18:54:02 dickey Exp $";
#endif

/*
 * Title:	wrepaint.c (repaint-window)
 * Author:	T.E.Dickey
 * Created:	24 Aug 1989
 * Modified:
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
	auto	int	min_row	= win->_begy,
			min_col	= win->_begx,
			max_col = win->_maxx + min_col - 1;
	register int	col, c;

	while (row < LINES) {
		win->_firstch[row] = 0;
		win->_lastch [row] = win->_maxx - 1;
		for (col = min_col; col < max_col; col++) {
			if ((c = win->_y[row-min_row][col-min_col]) == '~')
				c = '?';
			else
				c = '~';
			curscr->_y[row][col] = c;
		}
		row++;
	}
	clearok(win, FALSE);
	(void)wrefresh(win);
}
