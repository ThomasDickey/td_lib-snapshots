/*
 * Title:	dumpwin.c (dump curses window)
 * Author:	T.E.Dickey
 * Created:	20 Apr 1988 (from code written 13 Nov 1987)
 * Modified:
 *		04 Sep 1995, make this work with bsd4.4 curses
 *		15 May 1995, removed _orig/_nextp -- too obscure to bother with
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		11 May 1988, corrected dump of 'firstch[]', 'lastch[]' arrays,
 *			     which show the pending first/last change columns
 *			     for a given row.
 *
 * Function:	Dumps the state of a curses window
 *
 * Notes:
 *		Some versions of curses have an off-by-one error in the
 *		_maxx/_maxy values.  This code cannot assume that.
 */

#define		STR_PTYPES
#include	"ptypes.h"
#include	"td_curse.h"
#include	<time.h>

MODULE_ID("$Id: dumpwin.c,v 12.15 1995/10/14 20:51:45 tom Exp $")

#define	OUT	FPRINTF

void	dumpwin(
	_ARX(WINDOW *,	w)
	_AR1(char *,	tag)
		)
	_DCL(WINDOW *,	w)
	_DCL(char *,	tag)
{
	char	fname[MAXPATHLEN],
		*s = pathcat(fname, gethome(), "dumpwin.out");
	FILE	*fp = fopen(s, "a+");
	int	j,k;

	if (fp) {
		time_t	now = time((time_t *)0);
		int	current_x;
		int	current_y;

		getyx(w, current_y, current_x);

		OUT(fp, "%s: %s", tag, ctime(&now));
		OUT(fp, "window @ %p (LINES=%d, COLS=%d)\n", w, LINES, COLS);

		OUT(fp, "   _cury:%d, _curx:%d\n", current_y, current_x);
		OUT(fp, "   _maxy:%d, _maxx:%d\n", w->_maxy, w->_maxx);
		OUT(fp, "   _begy:%d, _begx:%d\n", w->_begy, w->_begx);

#if CURSES_LIKE_NCURSES
		OUT(fp, "   _region %d..%d\n",     w->_regtop, w->_regbottom);
		OUT(fp, "   _pary:%d  _parx:%d\n", w->_pary, w->_parx);
#endif
#if CURSES_LIKE_BSD44
		OUT(fp, "   _flags:   %#x\n", w->flags);
		OUT(fp, "   _clear:   %#x\n", w->flags & __CLEAROK);
		OUT(fp, "   _flush:   %#x\n", w->flags & __FLUSH);
		OUT(fp, "   _leave:   %#x\n", w->flags & __LEAVEOK);
		OUT(fp, "   _scroll:  %#x\n", w->flags & __SCROLLOK);
#else
		OUT(fp, "   _flags:   %#x\n", w->_flags);
		OUT(fp, "   _clear:   %#x\n", w->_clear);
		OUT(fp, "   _scroll:  %#x\n", w->_scroll);
#if CURSES_LIKE_NCURSES
		OUT(fp, "   _leave:   %#x\n", w->_leaveok);
#else
		OUT(fp, "   _leave:   %#x\n", w->_leave);
#endif
#endif

		OUT(fp, "   _y @ %p\n", &(CursesLine(w,0)));
		for (j = 0; j < wMaxY(w); j++) {
			OUT(fp, "%8d) [%3d,%3d] %p: \"",
				j,
				CursesFirstCh(w,j),
				CursesLastCh(w,j), CursesLine(w,j));
			if (CursesLine(w,j) != 0) {
				for (k = 0; k < wMaxX(w); k++)
					dumpchr(fp, CursesData(w,j,k));
			}
			OUT(fp, "\"\n");
			FFLUSH(fp);
		}
		FCLOSE(fp);
	}
}
