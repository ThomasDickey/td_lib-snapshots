#ifndef	NO_IDENT
static	char	*Id = "$Id: wresize.c,v 12.3 1994/10/06 23:59:53 tom Exp $";
#endif

/*
 * Title:	wresize.c (WINDOW-resize)
 * Author:	T.E.Dickey
 * Created:	22 Jul 1994 (from 'resizwin.c')
 *
 * Function:	This reallocates a curses WINDOW struct to either shrink or
 *		grow to the specified new lines/columns.  If it grows, the new
 *		character cells are filled with blanks.  The application is
 *		responsible for repainting the blank area.
 */
#include "ptypes.h"
#include "td_curse.h"

#ifdef	lint
#define	_BODY(f,c)	static c *f(p,n) c *p; int n; { return(0); }
/*ARGSUSED */		_BODY(pc_ALLOC,chtype *)
/*ARGSUSED */		_BODY(c_ALLOC, chtype)
/*ARGSUSED */		_BODY(s_ALLOC, short)
#else
#define	pc_ALLOC(p,n)	DOALLOC(p,chtype *,n)
#define	c_ALLOC(p,n)	DOALLOC(p,chtype,n)
#define	s_ALLOC(p,n)	DOALLOC(p,short,n)
#endif

void	wresize(
	_ARX(WINDOW *,	w)
	_ARX(int,	ToLines)
	_AR1(int,	ToCols)
		)
	_DCL(WINDOW *,	w)
	_DCL(int,	ToLines)
	_DCL(int,	ToCols)
{
	register int	row;
	int	adjx	= w->_maxx - COLS;
	int	adjy	= w->_maxy - LINES;

	/*
	 * If the number of lines has changed, adjust the size of the overall
	 * vector:
	 */
	if (ToLines != LINES) {
		for (row = ToLines; row < LINES; row++)
			free((char *)(w->_y[row]));

		w->_y       = pc_ALLOC(w->_y,      ToLines);
		w->_firstch = s_ALLOC(w->_firstch, ToLines);
		w->_lastch  = s_ALLOC(w->_lastch,  ToLines);

		for (row = LINES; row < ToLines; row++) {
			w->_y[row] = 0;
			w->_firstch[row] = 0;
			w->_lastch [row] = ToCols - 1;
		}
	}

	/*
	 * Adjust the width of the columns:
	 */
	for (row = 0; row < ToLines; row++) {
		chtype	*s	= w->_y[row];
		int	begin	= (s == 0) ? 0 : COLS;
		int	end	= ToCols;

		if (ToCols != begin) {
			w->_y[row] = s = c_ALLOC(s, ToCols+1);
			s[end] = 0;
			if (end > begin) {
				w->_lastch[row] = end - 1;
				while (end-- > begin)
					s[end] = ' ';
			} else {
				w->_firstch[row] = begin;
			}
		}
	}

	/*
	 * Finally, adjust the parameters showing screen size and cursor
	 * position:
	 */
	w->_maxx = adjx + ToCols;  if (w->_curx >= ToCols)  w->_curx = 0;
	w->_maxy = adjy + ToLines; if (w->_cury >= ToLines) w->_cury = 0;
#if HAVE_LIBNCURSES
	w->_regtop    = 0;
	w->_regbottom = w->_maxy;
#endif
}
