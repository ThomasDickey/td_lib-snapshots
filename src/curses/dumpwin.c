#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: dumpwin.c,v 12.6 1994/07/04 22:50:01 tom Exp $";
#endif

/*
 * Title:	dumpwin.c (dump curses window)
 * Author:	T.E.Dickey
 * Created:	20 Apr 1988 (from code written 13 Nov 1987)
 * Modified:
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
 */

#define		STR_PTYPES
#include	"ptypes.h"
#include	"td_curse.h"
#include	<time.h>

#define	OUT	FPRINTF(fp,

void	dumpwin(
	_ARX(WINDOW *,	w)
	_AR1(char *,	tag)
		)
	_DCL(WINDOW *,	w)
	_DCL(char *,	tag)
{
	chtype	*p;
	char	fname[BUFSIZ],
		*s = strcat(strcpy(fname, getenv("HOME")), "/dumpwin.out");
	FILE	*fp = fopen(s, "a+");
	int	j,k;

	if (fp) {
	time_t	now = time((time_t *)0);
		OUT "%s: %s", tag, ctime(&now));
		OUT "window @ %p\n", w);

		OUT "   _cury:%d, _curx:%d\n", w->_cury, w->_curx);
		OUT "   _maxy:%d, _maxx:%d\n", w->_maxy, w->_maxx);
		OUT "   _begy:%d, _begx:%d\n", w->_begy, w->_begx);

#if !SYS5_CURSES
		OUT "   _orig:    %p\n", w->_orig);
		OUT "   _nextp:   %p\n", w->_nextp);
#endif
		OUT "   _flags:   %#x\n", w->_flags);
		OUT "   _clear:   %#x\n", w->_clear);
		OUT "   _leave:   %#x\n", w->_leave);
		OUT "   _scroll:  %#x\n", w->_scroll);

		OUT "   _y @ %p\n", w->_y);
		for (j = 0; j < w->_maxy; j++) {
			p = w->_y[j];
			OUT "%8d) [%3d,%3d] %p: \"",
				j, w->_firstch[j], w->_lastch[j], p);
			if (p)
				while ((k = *p++) != EOS)
					dumpchr(fp,k);
			OUT "\"\n");
			FFLUSH(fp);
		}
		FCLOSE(fp);
	}
}

