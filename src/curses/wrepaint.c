#ifndef	lint
static	char	what[] = "$Id: wrepaint.c,v 4.1 1989/08/25 08:13:42 dickey Exp $";
#endif	lint

/*
 * Title:	wrepaint.c (repaint-window)
 * Author:	T.E.Dickey
 * Created:	24 Aug 1989
 *
 * Function:	Given a starting row-number, forces the screen to be redrawn
 *		without clearing it.
 *
 * Arguments:	win	=> window to repaint
 *		row	starting row number.
 *
 * Returns:	nothing.
 */

#define		CUR_PTYPES
#include	"ptypes.h"

wrepaint(win, row)
WINDOW	*win;
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
