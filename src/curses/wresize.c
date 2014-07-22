/*
 * Title:	wresize.c (WINDOW-resize)
 * Author:	T.E.Dickey
 * Created:	22 Jul 1994 (from 'resizwin.c')
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		03 Sep 1995, integration with bsd4.4 curses
 *		02 Jul 1995, integration with ncurses 1.9.3
 *
 * Function:	This reallocates a curses WINDOW struct to either shrink or
 *		grow to the specified new lines/columns.  If it grows, the new
 *		character cells are filled with blanks.  The application is
 *		responsible for repainting the blank area.
 */
#include "ptypes.h"
#include "td_curse.h"

MODULE_ID("$Id: wresize.c,v 12.14 2014/07/22 13:51:29 tom Exp $")

#if defined(CURSES_LIKE_BSD)

#define	pc_ALLOC(p,n)	DOALLOC(p,chtype *,n)
#define	c_ALLOC(p,n)	DOALLOC(p,chtype,n)
#define	s_ALLOC(p,n)	DOALLOC(p,short,n)
#endif

#if defined(CURSES_LIKE_NCURSES)
#define	ld_ALLOC(p,n)	DOALLOC(p,struct ldat,n)
#define	c_ALLOC(p,n)	DOALLOC(p,chtype,n)
#define	s_ALLOC(p,n)	DOALLOC(p,short,n)
#endif

#undef GOOD_RESIZE

#if !defined(HAVE_WRESIZE)
int
wresize(WINDOW *w,
	int ToLines,
	int ToCols)
{
#if defined(CURSES_LIKE_BSD) && !defined(GOOD_TOUCHWIN)
#define GOOD_WRESIZE 1
    int row;
    int adjx = w->_maxx - COLS;
    int adjy = w->_maxy - LINES;

    /*
     * If the number of lines has changed, adjust the size of the overall
     * vector:
     */
    if (ToLines != LINES) {
	for (row = ToLines; row < LINES; row++)
	    free((char *) (CursesLine(w, row)));

	w->_y = pc_ALLOC(w->_y, ToLines);
	w->_firstch = s_ALLOC(w->_firstch, ToLines);
	w->_lastch = s_ALLOC(w->_lastch, ToLines);

	for (row = LINES; row < ToLines; row++) {
	    CursesLine(w, row) = 0;
	    CursesFirstCh(w, row) = 0;
	    CursesLastCh(w, row) = ToCols - 1;
	}
    }

    /*
     * Adjust the width of the columns:
     */
    for (row = 0; row < ToLines; row++) {
	chtype *s = CursesLine(w, row);
	int begin = (s == 0) ? 0 : COLS;
	int end = ToCols;

	if (ToCols != begin) {
	    CursesLine(w, row) = s = c_ALLOC(s, ToCols + 1);
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
    w->_maxx = adjx + ToCols;
    if (w->_curx >= ToCols)
	w->_curx = 0;
    w->_maxy = adjy + ToLines;
    if (w->_cury >= ToLines)
	w->_cury = 0;
    return OK;
#endif /* CURSES_LIKE_BSD */

#if defined(CURSES_LIKE_BSD44)
#define GOOD_WRESIZE 1
    /*
     * This is based on "newwin.c 8.3 7/27/94" in the SlackWare 2.2 release.
     *
     * The simplest way to proceed is to allocate a new window, copy the
     * old data to it, and then reset the first/last changes so that only
     * the new area will be touched.  This relies on knowing that
     *
     *      the window's size won't be clipped by the values of LINES and
     *      COLS.
     *
     *      we know how to tinker with firstch/lastch
     *
     *      we know how to deallocate the temporary 'win' struct.
     *
     * We touch the whole window because some xterms (sigh) don't properly
     * repaint their bitmap when it's resized.
     */
    WINDOW *w2 = newwin(ToLines, ToCols, 0, 0);

    overlay(w, w2);
    touchwin(w2);		/* ...this isn't done automatically */

    free(w->lines);
    free(w->lspace);
    free(w->wspace);

    *w = *w2;
    w->nextp = w;		/* self-pointer */

    free((char *) w2);

    w->maxx = ToCols;
    if (w->curx >= ToCols)
	w->curx = 0;
    w->maxy = ToLines;
    if (w->cury >= ToLines)
	w->cury = 0;
    return OK;
#endif /* CURSES_LIKE_BSD44 */

#if defined(CURSES_LIKE_SYSV)
    /*
     * I don't know how to do this one.  I tried adapting the BSD-style
     * allocation, but got only as far as seeing that firstch/lastch are
     * apparently allocated as a single vector (/usr/5lib on SunOS).
     */
#endif

#if defined(CURSES_LIKE_NCURSES)
#define GOOD_WRESIZE 1
    int row;

    /*
     * If the number of lines has changed, adjust the size of the overall
     * vector:
     */
    if (ToLines != w->_maxy) {
	for (row = ToLines + 1; row <= w->_maxy; row++)
	    free((char *) (CursesLine(w, row)));

	w->_line = ld_ALLOC(w->_line, ToLines + 1);

	for (row = w->_maxy + 1; row <= ToLines; row++) {
	    CursesLine(w, row) = 0;
	    CursesFirstCh(w, row) = 0;
	    CursesLastCh(w, row) = ToCols;
	    w->_line[row].oldindex = NEWINDEX;
	}
    }

    /*
     * Adjust the width of the columns:
     */
    for (row = 0; row <= ToLines; row++) {
	chtype *s = CursesLine(w, row);
	int begin = (s == 0) ? 0 : w->_maxx;
	int end = ToCols;

	if (ToCols != begin) {
	    CursesLine(w, row) = s = c_ALLOC(s, ToCols + 1);
	    if (end > begin) {	/* growing */
		if (CursesFirstCh(w, row) < begin)
		    CursesFirstCh(w, row) = begin;
		CursesLastCh(w, row) = ToCols;
		do {
		    s[end] = ' ';
		} while (--end >= begin);
	    } else {		/* shrinking */
		CursesFirstCh(w, row) = 0;
		CursesLastCh(w, row) = ToCols;
	    }
	}
    }

    /*
     * Finally, adjust the parameters showing screen size and cursor
     * position:
     */
    w->_maxx = ToCols;
    if (w->_curx >= ToCols)
	w->_curx = 0;
    w->_maxy = ToLines;
    if (w->_cury >= ToLines)
	w->_cury = 0;
    return OK;
#endif

#if !defined(GOOD_RESIZE)
    return ERR;
#endif
}
#else
void dummy_wresize(void);
void
dummy_wresize(void)
{
}
#endif /* !HAVE_WRESIZE */
