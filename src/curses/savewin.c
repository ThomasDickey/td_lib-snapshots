/*
 * Author:	T.E.Dickey
 * Title:	savewin.c (save/unsave curses window)
 * Created:	25 Mar 1988
 * Modified:
 *		15 Feb 1998, cannot use this when chtype isn't scalar.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		07 Dec 1989, lint (SunOs 3.4)
 *		27 Jul 1989, recompiled with apollo SR10 -- mods for function
 *			     prototypes
 *		10 Aug 1988, made this run on apollo sys5-environment (hack).
 *		22 Apr 1988, added 'top' arg to lastwin(), unsavewin().
 *		21 Apr 1988 (first version)
 *
 * Function:	Save/unsave the curses window state on a stack (actually a
 *		linked list).  This is used to force a "nice" repaint on
 *		bsd4.3 curses systems, where 'touchwin()' doesn't work ok.
 *
 * Notes:	The bsd4.2 curses stores highlighting in the high-order bit of
 *		the screen-image characters.  When we do an 'addstr()' with
 *		this bit set, it retains highlighting.
 */

#include	"td_curse.h"
#include	<ctype.h>

MODULE_ID("$Id: savewin.c,v 12.8 1998/02/15 19:42:20 tom Exp $")

#if CURSES_LIKE_BSD && TYPE_CHTYPE_IS_SCALAR

typedef	struct	_save {
	struct	_save	*link;
	int		x,y;
	chtype		*image;
	} SAVE;

static	SAVE	*saved;

#ifdef	lint
#define	_BODY(f,c)	static c *f(n) unsigned n; { return(0); }
/*ARGSUSED */		_BODY(S_ALLOC, SAVE)
/*ARGSUSED */		_BODY(c_ALLOC, chtype)
#else
#define	S_ALLOC(n)	ALLOC(SAVE,n)
#define	c_ALLOC(n)	ALLOC(chtype,n)
#endif

/*
 * Force a character to be different
 */
static
int	newC(
	_AR1(int,c))
	_DCL(int,c)
{
	c++;
	if (!isprint(c))
		c = '.';
	if (c == ' ')
		c = '.';
	return (c);
}

/*
 * Save a window on the stack.
 */
void	savewin(_AR0)
{
	register int row, col;
	SAVE	*last;
	register int j = 0;

	last         = saved;
	saved        = S_ALLOC(1);
	saved->image = c_ALLOC((unsigned)(LINES * COLS));
	saved->link  = last;
	getyx(stdscr, saved->y, saved->x);

	for (row = 0; row < LINES; row++) {
		chtype	*src = CursesLine(stdscr,row);

		for (col = 0; col < COLS; col++)
			saved->image[j++] = *src++;
	}
}

/*
 * Restore the state of the last window saved on the stack.
 */
void	lastwin(
	_ARX(int,	redo)
	_AR1(int,	top)
		)
	_DCL(int,	redo)
	_DCL(int,	top)
{
	chtype	*t,
		*z = saved->image + (top * COLS);
	char	bfr[BUFSIZ];
	register int j, row;

	if (saved) {

		if (redo) {
			/* "touch" cursor position */
			(void)wmove(stdscr, LINES, COLS);
			(void)wmove(curscr, LINES, COLS);

			/* do "touch" pass first to avoid clrtoeol bug */
			for (row = top, t = z; row < LINES; row++) {
				chtype	*s;

				/* retrieve saved-image */
				for (j = 0; j < COLS; bfr[j++] = *t++);
				bfr[j] = EOS;

				/*
				 * Change curses' window state
				 * ...leaving last column alone because of
				 * wrap-forward bug!
				 */
#define	FOR_ROW(w,row)	for (s = w->_y[row], j = 0; (s != 0) && s[j+1]; j++)
				FOR_ROW(stdscr,row)
					s[j] = newC(bfr[j]);
				FOR_ROW(curscr,row)
					s[j] = newC(newC(bfr[j]));
			}
		}

		for (row = top, t = z; row < LINES; row++) {

			/* retrieve saved-image */
			for (j = 0; j < COLS; bfr[j++] = *t++)
				;
			bfr[j] = EOS;
			while ((--j >= 0) && (bfr[j] == ' '))
				bfr[j] = EOS;

			/* ...and then restore it */
			move(row,0);
			if (*bfr) addstr(bfr);
			/*else	addstr(" ");*/
			clrtoeol();
		}
		move(saved->y, saved->x);
		refresh();
	}
}

/*
 * Restore the last window, and pop it from the stack.
 */
void	unsavewin(
	_ARX(int,	redo)
	_AR1(int,	top)
		)
	_DCL(int,	redo)
	_DCL(int,	top)
{
	SAVE	*last;

	if (saved) {
		lastwin(redo,top);
		last = saved->link;
		dofree((char *)saved->image);
		dofree((char *)saved);
		saved = last;
	}
}

#endif /* CURSES_LIKE_BSD */
