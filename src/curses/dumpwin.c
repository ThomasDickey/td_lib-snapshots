#ifndef	lint
static	char	sccs_id[] = "@(#)dumpwin.c	1.2 88/04/21 14:00:26";
#endif	lint

/*
 * Title:	dumpwin.c (dump curses window)
 * Author:	T.E.Dickey
 * Created:	20 Apr 1988 (from code written 13 Nov 1987)
 * Modified:
 *
 * Function:	Dumps the state of a curses window
 *
 */

#include	<curses.h>
extern	long	time();
extern	char	*ctime(),
		*getenv(),
		*strcat(),
		*strcpy();

#define	OUT	(void)fprintf(fp,

dumpwin(w, tag)
WINDOW	*w;
char	*tag;
{
char	fname[BUFSIZ],
	*s = strcat(strcpy(fname, getenv("HOME")), "/dumpwin.out");
FILE	*fp = fopen(s, "a+");
int	j,k;
short	*t;

	if (fp) {
	long	now = time(0);
		OUT "%s: %s", tag, ctime(&now));
		OUT "window @ %#x\n", w);

		OUT "   _cury:%d, _curx:%d\n", w->_cury, w->_curx);
		OUT "   _maxy:%d, _maxx:%d\n", w->_maxy, w->_maxx);
		OUT "   _begy:%d, _begx:%d\n", w->_begy, w->_begx);

		OUT "   _firstch: %#x\n", w->_firstch);
		OUT "   _lastch:  %#x\n", w->_lastch);

		for (t = w->_firstch; t < w->_lastch; t++) {
			OUT "   %d) \"", t - w->_firstch);
			dumpchr(fp, (((*t) >> 8) & 0xff));
			dumpchr(fp, ((*t) & 0xff));
			OUT "\"\n");
		}

		OUT "   _orig:    %#x\n", w->_orig);
		OUT "   _nextp:   %#x\n", w->_nextp);
		OUT "   _flags:   %#x\n", w->_flags);
		OUT "   _clear:   %#x\n", w->_clear);
		OUT "   _leave:   %#x\n", w->_leave);
		OUT "   _scroll:  %#x\n", w->_scroll);

		OUT "   _y @ %#x\n", w->_y);
		for (j = 0; j < w->_maxy; j++) {
			s = w->_y[j];
			OUT "%8d) [%#x] %#x: \"", j, &w->_y[j], s);
			if (s)
				while (k = *s++)
					dumpchr(fp,k);
			OUT "\"\n");
			fflush(fp);
		}
		(void)fclose(fp);
	}
}
