#ifndef	lint
static	char	sccs_id[] = "@(#)savewin.c	1.4 88/05/17 09:25:22";
#endif	lint

/*
 * Author:	T.E.Dickey
 * Title:	savewin.c (save/unsave curses window)
 * Created:	25 Mar 1988
 * Modified:
 *		22 Apr 1988, added 'top' arg to lastwin(), unsavewin().
 *		21 Apr 1988 (first version)
 *
 * Function:	Save/unsave the curses window state on a stack (actually a
 *		linked list).
 *
 * Notes:	The bsd4.2 curses stores highlighting in the high-order bit of
 *		the screen-image characters.  When we do an 'addstr()' with
 *		this bit set, it retains highlighting.
 */
#include	<curses.h>
#include	<ctype.h>
extern	char	*doalloc();

#ifndef	SYSTEM5
typedef char	chtype;		/* sys5-curses data-type */
#endif	SYSTEM5

typedef	struct	_save {
	struct	_save	*link;
	int		x,y;
	chtype		*image;
	} SAVE;

static	SAVE	*saved;

#define	highlighted(c)	((c) & 0200)
#define	unhighlight(c)	((c) & 0177)

#ifdef	lint
#define	ALLOC(n,c)	(c *)0
#else	lint
#define	ALLOC(n,c)	(c *)doalloc((char *)0, (n) * sizeof(c))
#endif	lint

/*
 * Force a character to be different
 */
static
newC(c)
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
savewin()
{
register int row, col;
SAVE	*last;
register int j = 0;

	last         = saved;
	saved        = ALLOC(1, SAVE);
	saved->image = ALLOC(LINES * COLS, chtype);
	saved->link  = last;
	getyx(stdscr, saved->y, saved->x);

	for (row = 0; row < LINES; row++) {
	chtype	*src = stdscr->_y[row];
		for (col = 0; col < COLS; col++)
			saved->image[j++] = *src++;
	}
}

/*
 * Restore the state of the last window saved on the stack.
 */
lastwin(redo,top)
{
chtype	*s, *t,
	*z = saved->image + (top * COLS);
char	bfr[BUFSIZ];
register int j, row;

	if (saved) {

		if (redo) {
			/* "touch" cursor position */
			wmove(stdscr, LINES, COLS);
			wmove(curscr, LINES, COLS);

			/* do "touch" pass first to avoid clrtoeol bug */
			for (row = top, t = z; row < LINES; row++) {
				/* retrieve saved-image */
				for (j = 0; j < COLS; bfr[j++] = *t++);
				bfr[j] = '\0';

				/*
				 * Change curses' window state
				 * ...leaving last column alone because of
				 * wrap-forward bug!
				 */
				for (s = stdscr->_y[row], j = 0; s[j+1]; j++)
					s[j] = newC(bfr[j]);
				for (s = curscr->_y[row], j = 0; s[j+1]; j++)
					s[j] = newC(newC(bfr[j]));
			}
		}

		for (row = top, t = z; row < LINES; row++) {

			/* retrieve saved-image */
			for (j = 0; j < COLS; bfr[j++] = *t++);
			bfr[j] = '\0';
			while ((--j >= 0) && (bfr[j] == ' '))
				bfr[j] = '\0';

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
unsavewin(redo,top)
{
SAVE	*last;
	if (saved) {
		lastwin(redo,top);
		last = saved->link;
		dofree(saved->image);
		dofree((char *)saved);
		saved = last;
	}
}
