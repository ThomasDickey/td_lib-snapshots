#ifndef	lint
static	char	Id[] = "$Id: win2file.c,v 12.0 1991/10/03 16:27:12 ste_cm Rel $";
#endif

/*
 * Title:	win2file.c (window-to-file dump)
 * Author:	T.E.Dickey
 * Created:	07 Jun 1988
 * Modified:
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		23 Jan 1990, limit columns to dump by 'maxx' member (needed for
 *			     sun)
 *		03 Aug 1989, rewrote to add entrypoint 'win2fp()', which writes
 *			     to an already-open file-pointer.  Also, for the
 *			     new entrypoint, added a "prefix" argument which is
 *			     prepended to each line of the dumped window.
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
#include	<time.h>

#define	OUT	FPRINTF(fp,

static
mark(
_ARX(WINDOW *,	win)
_ARX(int,	row)
_ARX(int,	c)
_AR1(int,	bold)
	)
_DCL(WINDOW *,	win)
_DCL(int,	row)
_DCL(int,	c)
_DCL(int,	bold)
{
	(void)wmove(win, row + win->_begy, win->_begx);
	if (bold)	(void)wstandout(win);
	(void)waddch(win,c);
	if (bold)	(void)wstandend(win);
}

win2fp(
_ARX(WINDOW *,	win)
_ARX(FILE *,	fp)
_AR1(char *,	prefix)
	)
_DCL(WINDOW *,	win)
_DCL(FILE *,	fp)
_DCL(char *,	prefix)
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

win2file(
_ARX(WINDOW *,	win)
_AR1(char *,	file)
	)
_DCL(WINDOW *,	win)
_DCL(char *,	file)
{
	auto	FILE	*fp;

	if (fp = fopen(file, "a+")) {
		win2fp(win, fp, "");
		FCLOSE(fp);
		return (0);
	}
	return (-1);
}
