#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: resizwin.c,v 12.2 1993/10/29 17:35:24 dickey Exp $";
#endif

/*
 * Author:	T.E.Dickey
 * Title:	resizewin.c (change size of curses window)
 * Created:	21 Apr 1988
 * Modified:
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
 * Function:	inquire to see if the terminal's screen has changed
 *		size since curses was initialized.  If so, adjust stdscr and
 *		curscr to match.
 *
 * Returns:	TRUE if a change has been made.
 */

#include	"td_curse.h"

static	unsigned	size[2];

#define	my_LINES	size[0]
#define	my_COLS		size[1]

#ifdef	lint
#define	_BODY(f,c)	static c *f(p,n) c *p; unsigned n; { return(0); }
/*ARGSUSED */		_BODY(pc_ALLOC,chtype *)
/*ARGSUSED */		_BODY(c_ALLOC, chtype)
/*ARGSUSED */		_BODY(s_ALLOC, short)
#else
#define	pc_ALLOC(p,n)	DOALLOC(p,chtype *,n)
#define	c_ALLOC(p,n)	DOALLOC(p,chtype,n)
#define	s_ALLOC(p,n)	DOALLOC(p,short,n)
#endif

static
void	doit(
	_AR1(WINDOW *,	w))
	_DCL(WINDOW *,	w)
{
	register int	row;


	/*
	 * If the number of lines has changed, adjust the size of the overall
	 * vector:
	 */
	if (my_LINES != LINES) {
		for (row = my_LINES; row < LINES; row++)
			free((char *)(w->_y[row]));

		w->_y       = pc_ALLOC(w->_y,      my_LINES);
		w->_firstch = s_ALLOC(w->_firstch, my_LINES);
		w->_lastch  = s_ALLOC(w->_lastch,  my_LINES);

		for (row = LINES; row < my_LINES; row++)
			w->_y[row] = 0;
	}

	/*
	 * Adjust the width of the columns:
	 */
	for (row = 0; row < my_LINES; row++) {
	chtype	*s	= w->_y[row];
	int	begin	= (s == 0) ? 0 : COLS,
		end	= my_COLS;

		if (my_COLS != begin) {
			w->_y[row] = s = c_ALLOC(s, my_COLS+1);
			s[end] = 0;
			while (end-- > begin)
				s[end] = ' ';
		}
	}

	/*
	 * Finally, adjust the parameters showing screen size and cursor
	 * position:
	 */
	w->_maxx = my_COLS;  if (w->_curx >= my_COLS)  w->_curx = 0;
	w->_maxy = my_LINES; if (w->_cury >= my_LINES) w->_cury = 0;
}

int	resizewin(_AR0)
{
	int	lc[2];
	if (scr_size(lc) >= 0) {
		my_LINES = lc[0];
		my_COLS  = lc[1];
		if (my_LINES != LINES || my_COLS != COLS) {
			doit(stdscr);
			doit(curscr);
			LINES = my_LINES;
			COLS  = my_COLS;
			savewin();
			unsavewin(TRUE,0);
			return (1);
		}
	} else
		beep();
	return (0);
}
