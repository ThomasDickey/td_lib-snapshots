#ifndef	lint
static	char	sccs_id[] = "@(#)resizewin.c	1.6 88/05/17 14:00:11";
#endif	lint

/*
 * Author:	T.E.Dickey
 * Title:	resizewin.c (change size of curses window)
 * Created:	21 Apr 1988
 * Modified:
 *		13 May 1988, oops: was using old COLS, not new my_COLS to
 *			     reallocate row-strings.
 *		11 May 1988, reallocate firstch/lastch arrays (did not know what
 *			     they were til looking at curses source).
 *
 * Function:	(Apollo only) inquire to see if the VT100 window has changed
 *		size since curses was initialized.  If so, adjust stdscr and
 *		curscr to match.
 *
 * Notes:	A 'popen()' does not work because Apollo puts it into the
 *		wrong environment, so I made this hack using an undocumented
 *		call...
 *
 * Returns:	TRUE if a change has been made.
 */

#ifdef	apollo
#include </sys/ins/base.ins.c>
#include	<curses.h>
extern	char	*doalloc();
#endif	apollo

#ifndef	SYSTEM5
typedef char	chtype;		/* sys5-curses data-type */
#endif	SYSTEM5

#ifdef	apollo
static	short	size[2];

#define	my_LINES	size[0]
#define	my_COLS		size[1]

#ifdef	lint
#define	ALLOC(p,c,n)	p = (p+n)
#else	lint
#define	ALLOC(p,c,n)	p = (c *)doalloc((char *)p, (unsigned)((n) * sizeof(c)))
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
			free(w->_y[row]);

		ALLOC(w->_y,chtype *, my_LINES);
		ALLOC(w->_firstch,short, my_LINES);
		ALLOC(w->_lastch, short, my_LINES);

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
			w->_y[row] = ALLOC(s, chtype, my_COLS+1);
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
#endif	apollo

resizewin()
{
#ifdef	apollo
extern	void	vte_$inq_screen_size();
stream_$id_t	id	= ios_$stdout;
status_$t	st;

struct	{
	short	height;
	short	width;
	} screen_size;

	vte_$inq_screen_size(&id, &screen_size, &st);
	if (st.all == status_$ok) {
		size[0] = screen_size.height;
		size[1] = screen_size.width;
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
#endif	apollo
	return (0);
}
