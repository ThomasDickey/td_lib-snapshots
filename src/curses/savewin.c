#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/curses/RCS/savewin.c,v 5.0 1989/07/25 09:26:47 ste_cm Rel $";
#endif	lint

/*
 * Author:	T.E.Dickey
 * Title:	savewin.c (save/unsave curses window)
 * Created:	25 Mar 1988
 * $Log: savewin.c,v $
 * Revision 5.0  1989/07/25 09:26:47  ste_cm
 * BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *
 *		Revision 4.0  89/07/25  09:26:47  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.1  89/07/25  09:26:47  dickey
 *		recompiled with apollo SR10 -- mods for function prototypes
 *		
 *		Revision 3.0  88/08/11  07:16:55  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/11  07:16:55  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.6  88/08/11  07:16:55  dickey
 *		sccs2rcs keywords
 *		
 *		10 Aug 1988, made this run on apollo sys5-environment (hack).
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

#define		CUR_PTYPES
#include	"ptypes.h"
#include	<ctype.h>

typedef	struct	_save {
	struct	_save	*link;
	int		x,y;
	chtype		*image;
	} SAVE;

static	SAVE	*saved;

#define	highlighted(c)	((c) & 0200)
#define	unhighlight(c)	((c) & 0177)

#ifdef	lint
#define	_BODY(f,c)	static c *f(n) unsigned n; { return(0); }
/*ARGSUSED */		_BODY(S_ALLOC, SAVE)
/*ARGSUSED */		_BODY(c_ALLOC, chtype)
#else	lint
#define	S_ALLOC(n)	ALLOC(SAVE,n)
#define	c_ALLOC(n)	ALLOC(chtype,n)
#endif	lint

#ifndef	SYSTEM5
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
#endif	SYSTEM5

/*
 * Save a window on the stack.
 */
savewin()
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
	chtype	*src = stdscr->_y[row];
		for (col = 0; col < COLS; col++)
			saved->image[j++] = *src++;
	}
}

/*
 * Restore the state of the last window saved on the stack.
 */
lastwin(redo,top)
int	redo,top;
{
chtype	*t,
	*z = saved->image + (top * COLS);
char	bfr[BUFSIZ];
register int j, row;

	if (saved) {

		if (redo) {
#ifdef	SYSTEM5
			touchwin(stdscr);
			clear();
			refresh();
#else	SYSTEM5
			/* "touch" cursor position */
			wmove(stdscr, LINES, COLS);
			wmove(curscr, LINES, COLS);

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
#endif	SYSTEM5
		}

		for (row = top, t = z; row < LINES; row++) {

			/* retrieve saved-image */
			for (j = 0; j < COLS; bfr[j++] = *t++);
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
unsavewin(redo,top)
int	redo,top;
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
