#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/curses/RCS/resizwin.c,v 8.0 1988/08/11 07:39:12 ste_cm Rel $";
#endif	lint

/*
 * Author:	T.E.Dickey
 * Title:	resizewin.c (change size of curses window)
 * Created:	21 Apr 1988
 * $Log: resizwin.c,v $
 * Revision 8.0  1988/08/11 07:39:12  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  88/08/11  07:39:12  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/08/11  07:39:12  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/08/11  07:39:12  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/08/11  07:39:12  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/08/11  07:39:12  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/11  07:39:12  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.9  88/08/11  07:39:12  dickey
 *		sccs2rcs keywords
 *		
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

#define		CUR_PTYPES
#include	"ptypes.h"

static	unsigned	size[2];

#define	my_LINES	size[0]
#define	my_COLS		size[1]

#ifdef	lint
#define	_BODY(f,c)	static c *f(p,n) c *p; unsigned n; { return(0); }
/*ARGSUSED */		_BODY(pc_ALLOC,chtype *)
/*ARGSUSED */		_BODY(c_ALLOC, chtype)
/*ARGSUSED */		_BODY(s_ALLOC, short)
#else	lint
#define	pc_ALLOC(p,n)	DOALLOC(p,chtype *,n)
#define	c_ALLOC(p,n)	DOALLOC(p,chtype,n)
#define	s_ALLOC(p,n)	DOALLOC(p,short,n)
#endif	lint

static
doit(w)
WINDOW	*w;
{
register
int	row;


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

resizewin()
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
