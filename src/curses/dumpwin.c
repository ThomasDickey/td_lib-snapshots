#ifndef	lint
static	char	Id[] = "$Id: dumpwin.c,v 12.0 1991/10/03 16:06:54 ste_cm Rel $";
#endif

/*
 * Title:	dumpwin.c (dump curses window)
 * Author:	T.E.Dickey
 * Created:	20 Apr 1988 (from code written 13 Nov 1987)
 * Modified:
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		11 May 1988, corrected dump of 'firstch[]', 'lastch[]' arrays,
 *			     which show the pending first/last change columns
 *			     for a given row.
 *
 * Function:	Dumps the state of a curses window
 *
 */

#define		CUR_PTYPES
#define		STR_PTYPES
#include	"ptypes.h"
#include	<time.h>

#define	OUT	FPRINTF(fp,

dumpwin(
_ARX(WINDOW *,	w)
_AR1(char *,	tag)
	)
_DCL(WINDOW *,	w)
_DCL(char *,	tag)
{
#ifdef	SYSTEM5
chtype	*p;
#else
char	*p;
#endif
char	fname[BUFSIZ],
	*s = strcat(strcpy(fname, getenv("HOME")), "/dumpwin.out");
FILE	*fp = fopen(s, "a+");
int	j,k;

	if (fp) {
	time_t	now = time((time_t *)0);
		OUT "%s: %s", tag, ctime(&now));
		OUT "window @ %#x\n", w);

		OUT "   _cury:%d, _curx:%d\n", w->_cury, w->_curx);
		OUT "   _maxy:%d, _maxx:%d\n", w->_maxy, w->_maxx);
		OUT "   _begy:%d, _begx:%d\n", w->_begy, w->_begx);

#ifndef	SYSTEM5
		OUT "   _orig:    %#x\n", w->_orig);
		OUT "   _nextp:   %#x\n", w->_nextp);
#endif
		OUT "   _flags:   %#x\n", w->_flags);
		OUT "   _clear:   %#x\n", w->_clear);
		OUT "   _leave:   %#x\n", w->_leave);
		OUT "   _scroll:  %#x\n", w->_scroll);

		OUT "   _y @ %#x\n", w->_y);
		for (j = 0; j < w->_maxy; j++) {
			p = w->_y[j];
			OUT "%8d) [%3d,%3d] %#x: \"",
				j, w->_firstch[j], w->_lastch[j], p);
			if (p)
				while (k = *p++)
					dumpchr(fp,k);
			OUT "\"\n");
			FFLUSH(fp);
		}
		FCLOSE(fp);
	}
}
