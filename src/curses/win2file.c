/*
 * Title:	win2file.c (window-to-file dump)
 * Author:	T.E.Dickey
 * Created:	07 Jun 1988
 * Modified:
 *		15 Feb 1998, workaround for non-scalar chtype.
 *		03 Sep 1995, make this work with bsd4.4 curses
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

MODULE_ID("$Id: win2file.c,v 12.11 1998/02/15 18:54:47 tom Exp $")

#define	OUT	FPRINTF(fp,

#ifndef A_ALTCHARSET
#define A_ALTCHARSET 0
#endif

#ifndef A_CHARTEXT
#define A_CHARTEXT   0xff
#endif

#ifndef A_STANDOUT
#define A_STANDOUT   0200
#endif

#if TYPE_CHTYPE_IS_SCALAR
#  if CURSES_LIKE_BSD44
#    define CursesBold(win,y,x) CursesLine(win,y)[x].attr
#  else
#    define CursesBold(win,y,x) CursesData(win,y,x) & A_STANDOUT
#  endif
#else
#  define CursesBold(win,y,x) FALSE
#  undef CursesData
#  undef CursesLine
#endif

#ifndef CursesLine
static char *
CursesLine(WINDOW *win, int row)
{
	static char *result;

	if (result != 0)
		free(result);

	if ((result = malloc(COLS)) != 0) {
		int y, x;

		getyx(win, y, x);
		wmove(win, row, 0);
		winnstr(win, result, COLS);
		wmove(win, y, x);
	}
	return result;
}

#define CursesData(win,y,x)  CursesLine(win,y)[x]
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
	(void)wmove(win, (int)(row + win->_begy), (int)(win->_begx));
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

	register int	j, k;
	register chtype	khr;
	int	rows = wMaxY(win);
	int	cols = wMaxX(win);

	OUT "%sscreen saved at %s", *prefix ? prefix : "\f", ctime(&now));
	OUT "%s----------------(%dx%d)\n", prefix, rows, cols);

	getyx(win, y, x);
	for (j = 0; j < rows; j++) {
		OUT "%s", prefix);
		if (CursesLine(win,j) != NULL) {
			int	last = -1;

			/* animate this so user can see something */
			khr = CursesData(win,j,0);
			MarkIt(win, j, A_STANDOUT | '*');
			(void)wrefresh(win);
			MarkIt(win, j, khr);

			/* find the last nonblank column */
			for (k = 0; k < cols; k++) {
				khr = CursesData(win,j,k);
				if ((khr = toascii(khr)) == EOS)
					break;
				if (!isspace(khr))
					last = k;
			}

			/* dump the line, setting boldface as needed */
			for (k = 0; k <= last; k++) {
				auto	int	bold;

				khr = CursesData(win,j,k);
				bold = CursesBold(win,j,k);
				khr &= (A_CHARTEXT|A_ALTCHARSET);

#ifdef ACS_HLINE /* figure we've got the others */
				if (khr == ACS_HLINE)
					khr = '-';
				else if (khr == ACS_VLINE)
					khr = '|';
				else if (khr == ACS_ULCORNER
				   ||	 khr == ACS_LLCORNER
				   ||	 khr == ACS_URCORNER
				   ||	 khr == ACS_LRCORNER
				   ||	 khr == ACS_RTEE
				   ||	 khr == ACS_LTEE
				   ||	 khr == ACS_BTEE
				   ||	 khr == ACS_TTEE
				   ||	 khr == ACS_PLUS)
				   	khr = '+';
					else khr = toascii(khr);
#else
				khr = toascii(khr);
#endif
				if (isprint(khr)) {
					if (bold)
						OUT "%c\b", (int)khr);
					OUT "%c", (int)khr);
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
