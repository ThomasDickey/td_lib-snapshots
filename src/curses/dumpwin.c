#ifndef	lint
static	char	Id[] = "$Id: dumpwin.c,v 9.0 1991/05/15 10:02:08 ste_cm Rel $";
#endif

/*
 * Title:	dumpwin.c (dump curses window)
 * Author:	T.E.Dickey
 * Created:	20 Apr 1988 (from code written 13 Nov 1987)
 * $Log: dumpwin.c,v $
 * Revision 9.0  1991/05/15 10:02:08  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  10:02:08  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  88/08/11  07:13:54  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  88/08/11  07:13:54  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/08/11  07:13:54  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/08/11  07:13:54  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/08/11  07:13:54  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/08/11  07:13:54  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/11  07:13:54  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.7  88/08/11  07:13:54  dickey
 *		sccs2rcs keywords
 *		
 *		11 May 1988, corrected dump of 'firstch[]', 'lastch[]' arrays,
 *			     which show the pending first/last change columns
 *			     for a given row.
 *
 * Function:	Dumps the state of a curses window
 *
 */

#define		CUR_PTYPES
#include	"ptypes.h"
extern	time_t	time();
extern	char	*ctime(),
		*getenv(),
		*strcat(),
		*strcpy();

#define	OUT	(void)fprintf(fp,

dumpwin(w, tag)
WINDOW	*w;
char	*tag;
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
			(void)fflush(fp);
		}
		(void)fclose(fp);
	}
}
