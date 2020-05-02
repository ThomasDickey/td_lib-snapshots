/*
 * Title:	win2file.c (window-to-file dump)
 * Author:	T.E.Dickey
 * Created:	07 Jun 1988
 * Modified:
 *		01 May 2020, coverity warnings
 *              21 Nov 2017, increase buffer-size for trailing nul.
 *		07 Mar 2004, remove K&R support, indent'd.
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

#define CHR_PTYPES
#include	"td_curse.h"
#include	<ctype.h>
#include	<time.h>

MODULE_ID("$Id: win2file.c,v 12.25 2020/05/02 00:43:30 tom Exp $")

#ifndef A_ALTCHARSET
#define A_ALTCHARSET 0
#endif

#ifndef A_CHARTEXT
#define A_CHARTEXT   0xff
#endif

#ifndef A_STANDOUT
#define A_STANDOUT   0200
#endif

#if defined(HAVE_TYPE_CCHAR_T)
#undef TYPE_CHTYPE_IS_SCALAR
#endif

#if defined(TYPE_CHTYPE_IS_SCALAR) || !defined(HAVE_TYPE_CHTYPE)
#  if defined(CURSES_LIKE_BSD44)
#    define CursesBold(win,y,x) CursesLine(win,y)[x].attr
#  else
#    define CursesBold(win,y,x) CursesData(win,y,x) & A_STANDOUT
#  endif
#else
#  if !defined(HAVE_TYPE_CCHAR_T)
#    define CursesBold(win,y,x) FALSE
#  endif
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

    if ((result = malloc((size_t) (COLS + 1))) != 0) {
	int y, x;

	getyx(win, y, x);
	wmove(win, row, 0);
	winnstr(win, result, COLS);
	wmove(win, y, x);
    }
    return result;
}

#undef CursesData
#define CursesData(win,y,x)  (unsigned char)CursesLine(win,y)[x]
#endif

#ifndef CursesBold
static int
CursesBold(WINDOW *win, int y, int x)
{
    int save_y, save_x;
    int result = FALSE;
    getyx(win, save_y, save_x);
#if defined(HAVE_WIN_WCH)
    {
	cchar_t data;
	if (mvwin_wch(win, y, x, &data) == OK) {
	    wchar_t wch[10];
	    attr_t attrs;
	    short pair;
	    getcchar(&data, wch, &attrs, &pair, NULL);
	    if ((attrs & A_BOLD) != 0)
		result = TRUE;
	}
    }
#elif defined(HAVE_WINCH)
    {
	chtype data = mvwinch(win, y, x);
	if ((data & A_BOLD) != 0)
	    result = TRUE;
    }
#endif
    wmove(win, save_y, save_x);
    return result;
}
#endif

static void
MarkIt(WINDOW *win, int row, chtype c)
{
    (void) wmove(win, (int) (row + wBegY(win)), (int) wBegX(win));
    (void) waddch(win, c);
}

void
win2fp(WINDOW *win,
       FILE *fp,
       const char *prefix)
{
    time_t now = time((time_t *) 0);
    int y, x;

    int j, k;
    chtype khr;
    int rows = wMaxY(win);
    int cols = wMaxX(win);

    FPRINTF(fp, "%sscreen saved at %s", *prefix ? prefix : "\f", ctime(&now));
    FPRINTF(fp, "%s----------------(%dx%d)\n", prefix, rows, cols);

    getyx(win, y, x);
    for (j = 0; j < rows; j++) {
	FPRINTF(fp, "%s", prefix);
	if (CursesLine(win, j) != NULL) {
	    int last = -1;

	    /* animate this so user can see something */
	    khr = CursesData(win, j, 0);
	    MarkIt(win, j, A_STANDOUT | '*');
	    (void) wrefresh(win);
	    MarkIt(win, j, khr);

	    /* find the last nonblank column */
	    for (k = 0; k < cols; k++) {
		khr = CursesData(win, j, k);
		if ((khr = toascii(khr)) == EOS)
		    break;
		if (!isspace(khr))
		    last = k;
	    }

	    /* dump the line, setting boldface as needed */
	    for (k = 0; k <= last; k++) {
		int bold;

		khr = CursesData(win, j, k);
		bold = CursesBold(win, j, k);
		khr &= (A_CHARTEXT | A_ALTCHARSET);

#ifdef ACS_HLINE		/* figure we've got the others */
		if (khr == ACS_HLINE)
		    khr = '-';
		else if (khr == ACS_VLINE)
		    khr = '|';
		else if (khr == ACS_ULCORNER
			 || khr == ACS_LLCORNER
			 || khr == ACS_URCORNER
			 || khr == ACS_LRCORNER
			 || khr == ACS_RTEE
			 || khr == ACS_LTEE
			 || khr == ACS_BTEE
			 || khr == ACS_TTEE
			 || khr == ACS_PLUS)
		    khr = '+';
		else
		    khr = toascii(khr);
#else
		khr = toascii(khr);
#endif
		if (isprint(khr)) {
		    if (bold)
			FPRINTF(fp, "%c\b", (int) khr);
		    FPRINTF(fp, "%c", (int) khr);
		} else
		    FPRINTF(fp, "?");
	    }
	}
	FPRINTF(fp, "\n");
    }
    (void) wmove(win, y, x);
    (void) wrefresh(win);
}

int
win2file(
	    WINDOW *win,
	    char *file)
{
    FILE *fp;

    if ((fp = fopen(file, "a+")) != NULL) {
	win2fp(win, fp, "");
	FCLOSE(fp);
	return (0);
    }
    return (-1);
}

/******************************************************************************/
#ifdef	TEST
_MAIN
{
    (void) argc;
    (void) argv;
    exit(EXIT_FAILURE);
    /*NOTREACHED */
}
#endif /* TEST */
