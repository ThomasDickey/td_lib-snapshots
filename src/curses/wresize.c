#ifndef	NO_IDENT
static	char	*Id = "$Id: wresize.c,v 12.8 1995/07/30 17:55:50 tom Exp $";
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

#if CURSES_LIKE_BSD
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
#endif

#if CURSES_LIKE_NCURSES
#ifdef	lint
#define	_BODY(f,c)	static c *f(p,n) c *p; int n; { return(0); }
/*ARGSUSED */		_BODY(ld_ALLOC,struct ldat)
/*ARGSUSED */		_BODY(c_ALLOC, chtype)
/*ARGSUSED */		_BODY(s_ALLOC, short)
#else
#define	ld_ALLOC(p,n)	DOALLOC(p,struct ldat,n)
#define	c_ALLOC(p,n)	DOALLOC(p,chtype,n)
#define	s_ALLOC(p,n)	DOALLOC(p,short,n)
#endif
#endif

#ifndef max
#define max(a,b) ((a)<(b)?(b):(a))
#endif

#if !HAVE_WRESIZE
int	wresize(
	_ARX(WINDOW *,	w)
	_ARX(int,	ToLines)
	_AR1(int,	ToCols)
		)
	_DCL(WINDOW *,	w)
	_DCL(int,	ToLines)
	_DCL(int,	ToCols)
{
#if CURSES_LIKE_BSD
	register int	row;
	int	adjx	= w->_maxx - COLS;
	int	adjy	= w->_maxy - LINES;

	/*
	 * If the number of lines has changed, adjust the size of the overall
	 * vector:
	 */
	if (ToLines != LINES) {
		for (row = ToLines; row < LINES; row++)
			free((char *)(CursesLine(w,row)));

		w->_y       = pc_ALLOC(w->_y,      ToLines);
		w->_firstch = s_ALLOC(w->_firstch, ToLines);
		w->_lastch  = s_ALLOC(w->_lastch,  ToLines);

		for (row = LINES; row < ToLines; row++) {
			CursesLine(w, row)    = 0;
			CursesFirstCh(w, row) = 0;
			CursesLastCh(w, row)  = ToCols - 1;
		}
	}

	/*
	 * Adjust the width of the columns:
	 */
	for (row = 0; row < ToLines; row++) {
		chtype	*s	= CursesLine(w,row);
		int	begin	= (s == 0) ? 0 : COLS;
		int	end	= ToCols;

		if (ToCols != begin) {
			CursesLine(w,row) = s = c_ALLOC(s, ToCols+1);
			s[end] = 0;
			if (end > begin) {
				CursesLastCh(w, row) = end - 1;
				while (end-- > begin)
					s[end] = ' ';
			} else {
				CursesFirstCh(w, row) = begin;
			}
		}
	}

	/*
	 * Finally, adjust the parameters showing screen size and cursor
	 * position:
	 */
	w->_maxx = adjx + ToCols;  if (w->_curx >= ToCols)  w->_curx = 0;
	w->_maxy = adjy + ToLines; if (w->_cury >= ToLines) w->_cury = 0;
	return OK;
#endif	/* CURSES_LIKE_BSD */

#if CURSES_LIKE_SYSV
	/*
	 * I don't know how to do this one.  I tried adapting the BSD-style
	 * allocation, but got only as far as seeing that firstch/lastch are
	 * apparently allocated as a single vector (/usr/5lib on SunOS).
	 */
#endif

#if CURSES_LIKE_NCURSES
	register int	row;

	/*
	 * If the number of lines has changed, adjust the size of the overall
	 * vector:
	 */
	if (ToLines != w->_maxy) {
		for (row = ToLines+1; row <= w->_maxy; row++)
			free((char *)(CursesLine(w,row)));

		w->_line  = ld_ALLOC(w->_line, ToLines+1);

		for (row = w->_maxy+1; row <= ToLines; row++) {
			CursesLine(w,row)    = 0;
			CursesFirstCh(w,row) = 0;
			CursesLastCh(w,row)  = ToCols;
			w->_line[row].oldindex = NEWINDEX;
		}
	}

	/*
	 * Adjust the width of the columns:
	 */
	for (row = 0; row <= ToLines; row++) {
		chtype	*s	= CursesLine(w, row);
		int	begin	= (s == 0) ? 0 : w->_maxx;
		int	end	= ToCols;

		if (ToCols != begin) {
			CursesLine(w, row) = s = c_ALLOC(s, ToCols+1);
			if (end > begin) {	/* growing */
				if (CursesFirstCh(w, row) < begin)
					CursesFirstCh(w, row) = begin;
				CursesLastCh(w, row) = ToCols;
				do {
					s[end] = ' ';
				} while (--end >= begin);
			} else {		/* shrinking */
				CursesFirstCh(w, row) = 0;
				CursesLastCh(w, row)  = ToCols;
			}
		}
	}

	/*
	 * Finally, adjust the parameters showing screen size and cursor
	 * position:
	 */
	w->_maxx = ToCols;  if (w->_curx >= ToCols)  w->_curx = 0;
	w->_maxy = ToLines; if (w->_cury >= ToLines) w->_cury = 0;
	return OK;
#endif
}
#endif	/* !HAVE_WRESIZE */
