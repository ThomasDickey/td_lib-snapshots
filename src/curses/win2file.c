#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/curses/RCS/win2file.c,v 3.0 1988/08/11 08:42:33 ste_cm Rel $";
#endif	lint

/*
 * Title:	win2file.c (window-to-file dump)
 * Author:	T.E.Dickey
 * Created:	07 Jun 1988
 * $Log: win2file.c,v $
 * Revision 3.0  1988/08/11 08:42:33  ste_cm
 * BASELINE Mon Jun 19 13:27:01 EDT 1989
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
	if (bold)	standout();
	(void)waddch(win,c);
	if (bold)	standend();
}

win2file(win, file)
WINDOW	*win;
char	*file;
{
	FILE	*fp;
	time_t	now	= time((time_t *)0);
	register chtype	*s;
	register int	j;
	register int	k;
	int	y,x;

	if (fp = fopen(file, "a+")) {
		OUT "\fscreen saved at %s", ctime(&now));
		OUT "----------------\n");

		getyx(win, y, x);
		for (j = 0; j < win->_maxy; j++) {
			if (s = win->_y[j]) {
			chtype	*t = s;

				/* animate this so user can see something */
				k = *s;
				mark(win, j, '*', 1);
				wrefresh(win);
				mark(win, j, toascii(k), !isascii(k));

				/* find the last nonblank column */
				while (k = toascii(*s++)) {
					if (!isspace(k))
						t = s;
				}

				/* dump the line, setting boldface as needed */
				for (s = win->_y[j]; s < t; s++) {
					int	bold = !isascii(k = *s);

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
		wrefresh(win);
		FCLOSE(fp);
	}
}
