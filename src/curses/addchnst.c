#ifndef NO_IDENT
static char *Id = "$Id: addchnst.c,v 12.2 1995/01/28 14:01:15 tom Exp $";
#endif

/*
 * Title:	addchnst.c (addchnstr)
 * Author:	T.E.Dickey
 * Created:	28 Jan 1995
 *
 * Function:	Supplies the function 'waddchnstr()' for implementations of
 *		curses that don't have it (e.g., bsd curses, HPUX, AIX).
 *
 *		This leaves the current position after the end of the string.
 *		Some implementations of waddchnstr() leave the position at
 *		the original position.
 */

#include	"ptypes.h"
#include	"td_curse.h"

#ifndef OK
#define OK 0
#endif

#if !HAVE_ADDCHNSTR
int	waddchnstr(
	_ARX(WINDOW *,	win)
	_ARX(chtype *,	s)
	_AR1(int,	len)
		)
	_DCL(WINDOW *,	win)
	_DCL(chtype *,	s)
	_DCL(int,	len)
{
	int	code, y, x;

	getyx(win, y, x);
	if (len < 0) {
		for (len = 0; s[len] != 0; len++)
			;
	}
	while (len-- > 0) {
		if ((code = waddch(win, *s++)) != OK) {
			return code;
		}
		if (++x >= win->_maxx) {
			x = win->_begx;
			y++;
		}
		wmove(win, y, x);
	}
	return OK;
}
#endif
