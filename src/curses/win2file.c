#ifndef	lint
static	char	Id[] = "$Id: win2file.c,v 9.0 1991/05/15 09:59:06 ste_cm Rel $";
#endif

/*
 * Title:	win2file.c (window-to-file dump)
 * Author:	T.E.Dickey
 * Created:	07 Jun 1988
 * $Log: win2file.c,v $
 * Revision 9.0  1991/05/15 09:59:06  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:59:06  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  90/01/23  16:23:11  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  90/01/23  16:23:11  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  90/01/23  16:23:11  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.1  90/01/23  16:23:11  dickey
 *		limit columns to dump by 'maxx' member (needed for sun)
 *		
 *		Revision 5.0  89/08/03  14:54:57  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  89/08/03  14:54:57  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.1  89/08/03  14:54:57  dickey
 *		rewrote to add entrypoint 'win2fp()', which writes to an
 *		already-open file-pointer.  Also, for the new entrypoint,
 *		added a "prefix" argument which is prepended to each line
 *		of the dumped window.
 *		
 *		Revision 3.0  88/08/11  08:42:33  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/11  08:42:33  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.4  88/08/11  08:42:33  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Writes the contents of the given CURSES window 'win' to the
 *		specified file (appends if the file already exists).  Bold,
 *		underlined, other special items are translated to boldface
 *		(overstruck) in the output file.
 *
 *		As the screen is dumped, we make a highlighted mark move down
 *		the left margin, so that the user can see that something is
 *		happening.
 *
 * Arguments:	win	=> window to dump
 *		file	name of file to write the display to
 *
 * Returns:	nothing.
 */

#define		CUR_PTYPES
#include	"ptypes.h"
#include	<ctype.h>
extern	time_t	time();
extern	char	*ctime();

#define	OUT	FPRINTF(fp,

static
mark(win, row, c, bold)
WINDOW	*win;
{
	(void)wmove(win, row + win->_begy, win->_begx);
	if (bold)	(void)wstandout(win);
	(void)waddch(win,c);
	if (bold)	(void)wstandend(win);
}

win2fp(win, fp, prefix)
WINDOW	*win;
FILE	*fp;
char	*prefix;
{
	auto	time_t	now	= time((time_t *)0);
	auto	int	y,x;

	register chtype	*s;
	register int	j;
	register int	k;

	OUT "%sscreen saved at %s", *prefix ? prefix : "\f", ctime(&now));
	OUT "%s----------------\n", prefix);

	getyx(win, y, x);
	for (j = 0; j < win->_maxy; j++) {
		OUT "%s", prefix);
		if (s = win->_y[j]) {
			auto	chtype	*t = s;

			/* animate this so user can see something */
			k = *s;
			mark(win, j, '*', 1);
			(void)wrefresh(win);
			mark(win, j, toascii(k), !isascii(k));

			/* find the last nonblank column */
			while (k = toascii(*s++)) {
				if ((s - win->_y[j]) >= win->_maxx)
					break;
				if (!isspace(k))
					t = s;
			}

			/* dump the line, setting boldface as needed */
			for (s = win->_y[j]; s < t; s++) {
				auto	int	bold = !isascii(k = *s);

				k = toascii(k);
				if (isprint(k)) {
					if (bold)
						OUT "%c\b", k);
					OUT "%c", k);
				} else
					OUT "?");
			}
		}
		OUT "\n");
	}
	(void)wmove(win, y,x);
	(void)wrefresh(win);
}

win2file(win, file)
WINDOW	*win;
char	*file;
{
	auto	FILE	*fp;

	if (fp = fopen(file, "a+")) {
		win2fp(win, fp, "");
		FCLOSE(fp);
		return (0);
	}
	return (-1);
}
