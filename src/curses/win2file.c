#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: win2file.c,v 12.6 1994/10/18 22:53:42 tom Exp $";
#endif

/*
 * Title:	win2file.c (window-to-file dump)
 * Author:	T.E.Dickey
 * Created:	07 Jun 1988
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
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

#include	"td_curse.h"
#include	<ctype.h>
#include	<time.h>

#define	OUT	FPRINTF(fp,

#ifndef A_ATTRIBUTES
#define A_ATTRIBUTES 0200
#endif

#ifndef A_ALTCHARSET
#define A_ALTCHARSET 0
#endif

#ifndef A_CHARTEXT
#define A_CHARTEXT   0xff
#endif

#ifndef A_STANDOUT
#define A_STANDOUT   0200
#endif

static
void	MarkIt(
	_ARX(WINDOW *,	win)
	_ARX(int,	row)
	_AR1(chtype,	c)
		)
	_DCL(WINDOW *,	win)
	_DCL(int,	row)
	_DCL(chtype,	c)
{
	(void)wmove(win, row + win->_begy, win->_begx);
	(void)waddch(win,c);
}

void	win2fp(
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
	register chtype	k;
	int	rows = wMaxY(win);

	OUT "%sscreen saved at %s", *prefix ? prefix : "\f", ctime(&now));
	OUT "%s----------------\n", prefix);

	getyx(win, y, x);
	for (j = 0; j < rows; j++) {
		OUT "%s", prefix);
		if ((s = win->_y[j]) != NULL) {
			auto	chtype	*t = s;

			/* animate this so user can see something */
			k = *s;
			MarkIt(win, j, A_STANDOUT | '*');
			(void)wrefresh(win);
			MarkIt(win, j, k);

			/* find the last nonblank column */
			while ((k = toascii(*s++)) != EOS) {
				if ((s - win->_y[j]) >= wMaxX(win))
					break;
				if (!isspace(k))
					t = s;
			}

			/* dump the line, setting boldface as needed */
			for (s = win->_y[j]; s < t; s++) {
				auto	int	bold;

				k = *s;
				bold = (k & A_STANDOUT) != 0;
				k &= (A_CHARTEXT|A_ALTCHARSET);

#ifdef ACS_HLINE /* figure we've got the others */
				if (k == ACS_HLINE)
					k = '-';
				else if (k == ACS_VLINE)
					k = '|';
				else if (k == ACS_ULCORNER
				   ||	 k == ACS_LLCORNER
				   ||	 k == ACS_URCORNER
				   ||	 k == ACS_LRCORNER
				   ||	 k == ACS_RTEE
				   ||	 k == ACS_LTEE
				   ||	 k == ACS_BTEE
				   ||	 k == ACS_TTEE
				   ||	 k == ACS_PLUS)
				   	k = '+';
#if 1
					else k = toascii(k);
#else
				else if (!isascii(k))
					k = '*';
#endif /**/
#else
				k = toascii(k);
#endif
				if (isprint(k)) {
					if (bold)
						OUT "%c\b", (int)k);
					OUT "%c", (int)k);
				} else
					OUT "?");
			}
		}
		OUT "\n");
	}
	(void)wmove(win, y,x);
	(void)wrefresh(win);
}

int	win2file(
	_ARX(WINDOW *,	win)
	_AR1(char *,	file)
		)
	_DCL(WINDOW *,	win)
	_DCL(char *,	file)
{
	auto	FILE	*fp;

	if ((fp = fopen(file, "a+")) != NULL) {
		win2fp(win, fp, "");
		FCLOSE(fp);
		return (0);
	}
	return (-1);
}
