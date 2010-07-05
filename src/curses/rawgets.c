/*
 * Author:	T.E.Dickey
 * Title:	rawgets.c (raw-mode 'gets()')
 * Created:	29 Sep 1987 (from 'fl.c')
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		05 Feb 1996, don't write to lower-right corner (sysvr4 bug).
 *		04 Nov 1995, mods to display on 80th column.
 *		03 Sep 1995, make this work with bsd4.4 curses
 *		19 Jul 1994, adjustment for ncurses _max[xy] bug.
 *		16 Jul 1994, explicitly call for reverse-video if Sys5-curses.
 *		30 Jun 1994, added CTL/P, CTL/N as synonyms for up/down arrows
 *		28 Jun 1994, modified for window-resizing.
 *		30 May 1994, always allow backspace as an erase-character.
 *		24 Nov 1993, added xterm-mouse support.
 *		05 Nov 1993, absorb "cmdch.h" into "td_curse.h"
 *		29 Oct 1993, ifdef-ident
 *		28 Sep 1993, modified 'InsertAt()' to avoid reading past the
 *			     end of the 'at' string (makes Purify happy).
 *		21 Sep 1993, gcc-warnings
 *		04 Sep 1992, modified to allow nonprinting chars in buffer.
 *		25 Aug 1992, added 'first_mode' argument.
 *		20 Aug 1992, added 'field_len', 'first_col' arguments.
 *		17 Aug 1992, if 'fast_q' is non-null, start edit in scroll-mode
 *		10 Aug 1992, allow window-arg to be null, for replaying scripts
 *			     to buffers that are not visible.
 *		06 Aug 1992, added command/logging arguments.
 *		05 Aug 1992, highlight in insert-mode when wrap is disabled.
 *			     Added fast-quit and erase-word features.
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, mods to compile under apollo sr10.3
 *		02 Mar 1990, modified so that if this is invoked in no-wrap
 *			     mode, and the output buffer is wider than the
 *			     screen, we automatically scroll left/right.  Also,
 *			     permit arrow keys to work in non-insert mode.
 *			     Finally, added a test-driver to exercise the code.
 *		04 Oct 1989, lint (apollo SR10.1)
 *		03 Aug 1989, broke into two procedures, 'rawgets()' and
 *			     'wrawgets()'.  Return the terminating character,
 *			     allowing up/down arrow as one of these.  The
 *			     'wrawgets()' procedure operates solely in a
 *			     specified window; made additional modifications to
 *			     keep the wraparound and clearing under better
 *			     control.
 *		25 Jul 1989, recompiled with apollo SR10 -- mods for function
 *			     prototypes
 *		19 Jan 1989, move to end of string when exiting.
 *		28 Apr 1988, use CTL/B, CTL/F for inline movement
 *		27 Apr 1988, interfaced to 'cmdch()'.  General cleanup to fix
 *			     bugs in wraparound.
 *		24 Nov 1987, moved to my SPC library, use under 'curses'.
 *
 * Function:	Accept input from the screen, permitting a user to either
 *		alter an existing string, or to enter a new one.  This
 *		procedure is called in raw/noecho mode, and is used by the
 *		directory editor to enter new text.
 *
 *		Only printing (including space) characters will be returned
 *		in 'bfr[]'.
 *
 *		On entry, the caller has printed a prompt/tag to the screen
 *		(before the buffer to be returned), and the screen position
 *		is the end of the string.
 *
 *		On normal exit, the user provides a newline, which is echoed.
 */

#define		SIG_PTYPES	/* to pick up SIGWINCH, and 'on_winch()' */
#define		STR_PTYPES
#include	"ptypes.h"
#include	<ctype.h>
#include	"td_curse.h"
#include	"dyn_str.h"

MODULE_ID("$Id: rawgets.c,v 12.28 2010/07/04 21:45:47 tom Exp $")

#define	SHIFT	5

#ifndef KEY_MIN
#define KEY_MIN 256
#endif

#define is_special(c)	((c) >= KEY_MIN)
#define	to_toggle(c)	((c) == '\t')
#define	to_literal(c)	((c) == CTL('V'))
#define	to_home(c)	(((c) == CTL('B')))
#define	to_up(c)	(((c) == CTL('P')) || ((c) == KEY_UP))
#define	to_down(c)	(((c) == CTL('N')) || ((c) == KEY_DOWN))
#define	to_left(c)	(((c) == '\b') || ((c) == KEY_LEFT))
#define	to_right(c)	(((c) == '\f') || ((c) == KEY_RIGHT))
#define	to_end(c)	(((c) == CTL('F')))

#ifdef A_REVERSE
#define Highlight(w)	(void)wattron(w, A_REVERSE)
#define NoHighlight(w)	(void)wattroff(w, A_REVERSE)
#else
#define Highlight(w)	(void)wstandout(w)
#define NoHighlight(w)	(void)wstandend(w)
#endif

/*
 * Keep the base-position of 'bfr[]' visible, so that when resizing the
 * window, the calling application can move the 'rawgets()' display area.
 */
int x_rawgets, y_rawgets;

/*
 * Private functions/variables
 */
static void MoveTo(char *at);
static void ShowAt(char *at);

static WINDOW *Z;		/* window we use in this module */
static const char **Prefix;	/* insert/scrolling prefix, if any */
static DYN *history;		/* record of keystrokes if logging active */
static int xlast;		/* last usable column in screen */
static int shift;		/* amount shifted in no-wrap mode */
static int wrap;		/* if we echo newline, assume wrappable */
static int errs;		/* flag for error/illegal char */
static int Imode;		/* insert:1, scroll:0 */
static char *bbase;		/* 'bfr[]' copy */
static char *CurIns;		/* current insertion position */
static int FieldLen;		/* limit before truncation or wrap */

/*
 * Clear the remainder of the current line to the 'xlast' position.  Don't
 * use wclrtoeol() when wrapping, since xlast may not be on the end.
 */
static void
ClearIt(void)
{
    if (Z) {
	int x, y;
	int highlighted = (!wrap && !Imode);
	int limit;

	if (highlighted)
	    NoHighlight(Z);
	getyx(Z, y, x);
	limit = (y >= LINES - 1 && xlast >= COLS - 1) ? COLS - 2 : xlast;
	while (++x <= limit)
	    (void) waddch(Z, ' ');
	if (highlighted)
	    Highlight(Z);
    }
}

static void
ShowAll(void)
{
    if (Z) {
	(void) wmove(Z, y_rawgets, x_rawgets);
	ShowAt(bbase + shift);
    }
}

/*
 * Position the cursor at the given index in the string.  This permits us to
 * ignore details such as wraparound, and whether the cursor is at the end of
 * string.
 */
static void
MoveTo(char *at)
{
    if (Z) {
	char *s;
	int y = y_rawgets, x = x_rawgets;
	int original = shift;

	for (s = bbase, shift = 0; *s != EOS && s != at; s++) {
	    if (!isprint(UCH(*s)))
		x++;
	    if (++x >= xlast) {
		if (wrap) {
		    x = 0;
		    y++;
		} else {
		    shift += SHIFT;
		    x -= SHIFT;
		}
	    }
	}
	/* Control characters that are before the left shift-margin
	 * don't count in the adjustment
	 */
	for (s = bbase; s - bbase < shift; s++)
	    if (!isprint(UCH(*s)))
		x--;
	if (shift != original)
	    ShowAll();

	(void) wmove(Z, y, x);
    }
}

#ifndef	NO_XTERM_MOUSE
/*
 * Given cursor coordinates, computes the resulting position within the
 * buffer.  Update the display and return the buffer pointer.
 */
static char *
MoveFrom(int row, int col)
{
    if (Z) {
	char *s;
	int y = y_rawgets, x = x_rawgets;

	if (row < y_rawgets) {
	    row = y_rawgets;
	    col = x_rawgets;
	}

	for (s = bbase; *s != EOS; s++) {
	    if (y == row
		&& x == col)
		break;
	    if (!isprint(UCH(*s)))
		x++;
	    if (++x >= xlast) {
		if (wrap) {
		    x = 0;
		    y++;
		}
	    }
	}
	MoveTo(s);
	ShowAt(s);
	return s;
    }
    return 0;
}
#endif

/*
 * Repaint the string starting at a given position
 */
static void
ShowAt(char *at)
{
    if (Z) {
	int y, x, row, col, len, cnt;
	int margin = wMaxY(Z);

	getyx(Z, y, x);
	for (row = y, col = x; (*at != EOS) && (row < margin); row++) {
	    (void) wmove(Z, row, col);
	    len = (int) strlen(at);
	    cnt = xlast - col;
	    while (len-- > 0) {
		chtype c;
		if (cnt-- <= 0)
		    break;
		c = *at++ & 0xff;
		if (!isprint(c)) {
		    x++;
		    (void) waddch(Z, '^');
		    if (cnt-- <= 0)
			break;
		    if (c == '\177')
			c = '?';
		    else
			c |= '@';
		}
		(void) waddch(Z, c);
	    }
	    if (!wrap) {
		if (len > 0 || cnt <= 0) {
		    (void) wmove(Z, y, x);
		    return;
		}
		break;
	    }
	    col = 0;
	}
	ClearIt();
	(void) wmove(Z, y, x);
    }
}

/*
 * Insert a character in the screen and into 'bfr[]' at the given position.
 */
static void
InsertAt(char *at, int c)
{
    int d = c;
    char *s = at;

    for (;;) {
	if ((c = d) != EOS) {
	    d = *s;
	    *s++ = (char) c;
	} else
	    break;
    }
    *s = EOS;
    ShowAt(at);
    MoveTo(at + 1);
}

/*
 * Delete the character(s) before the given position in 'bfr[]'.
 */
static char *
DeleteBefore(char *at, int count)
{
    if (at > bbase) {
	char *d = at, *s = at;
	int old = 0, new_y, x;

	if (Z) {
	    if (wrap) {
		MoveTo(at + strlen(at));
		getyx(Z, old, x);
	    }
	}
	while (count-- > 0) {
	    at--;
	    if (--d == bbase)
		break;
	}

	while ((*d++ = *s++) != EOS)
	    /*EMPTY */ ;

	if (Z) {
	    if (wrap) {
		MoveTo(at + strlen(at));
		getyx(Z, new_y, x);

		while (old >= new_y) {
		    (void) wmove(Z, new_y, x);
		    ClearIt();
		    new_y++;
		    x = 0;
		}
	    }
	    MoveTo(at);
	    ShowAt(at);
	}
    } else
	errs++;

    return (at);
}

/*
 * Delete the words before the given pointer
 */
static char *
DeleteWordBefore(char *at, int count)
{
    char *s;
    int found;

    while ((at > bbase) && (count-- > 0)) {
	for (s = at - 1, found = 0; s >= bbase; s--) {
	    if (isspace(UCH(*s))) {
		if (found) {
		    s++;	/* point to first nonblank */
		    break;
		}
	    } else
		found++;
	}
	at = DeleteBefore(at, (int) (at - s));
    }
    return (at);
}

/*
 * Show the insert/scroll prefix
 */
static void
ShowPrefix(void)
{
    if (Z && Prefix) {
	const char *prefix = Prefix[Imode];

	(void) NoHighlight(Z);
	(void) wmove(Z, y_rawgets, (x_rawgets - (int) strlen(prefix)));
	while (*prefix)
	    (void) waddch(Z, (chtype) (*prefix++));
    }
}

/*
 * Toggle the insert/scroll mode, and show the state of this flag by
 * overwriting the prefix-string of the prompt which is written before
 * calling this procedure.
 */
static void
ToggleMode(void)
{
    Imode = !Imode;

    if (Z) {
	int y, x;

	getyx(Z, y, x);
	ShowPrefix();

	if (!wrap) {
	    if (!Imode)
		Highlight(Z);
	    ShowAll();
	}

	(void) wmove(Z, y, x);
    }
}

/*
 * This function is invoked after a window-resizing signal is received, while
 * waiting for input.  Repaint the buffer in case the window-width impacts its
 * layout.
 */
#ifdef	SIGWINCH
static void
Redisplay(void)
{
    WINDOW *win = Z;
    if (win == 0)
	win = stdscr;
    (void) wmove(win, y_rawgets, x_rawgets);
    if (wrap) {
	xlast = x_rawgets + FieldLen;
	if (xlast > wMaxX(Z))
	    xlast = wMaxX(Z);
	(void) wclrtobot(win);
	(void) wmove(win, y_rawgets, x_rawgets);
    } else {
	xlast = wMaxX(Z);
    }
    ShowPrefix();
    ShowAt(bbase);
    MoveTo(CurIns);
    (void) wrefresh(win);
}
#endif

/************************************************************************
 *	main procedure							*
 ************************************************************************/

int
wrawgets(WINDOW *win,
	 char *bfr,		/* in/out buffer */
	 const char **pref,	/* prefix, for insert/scroll */
	 int buffer_len,	/* maximum length of 'bfr' */
	 int field_len,		/* maximum length of display-field */
	 int first_col,		/* initial column for editing */
	 int first_mode,	/* initial insert/scroll mode */
	 int new_line,		/* force newline-echo on completion */
	 int fast_q,		/* nonnull: extra quit character */
	 char **command,	/* nonnull: read inputs */
	 int logging)		/* nonnull: write inputs */
{
    int c;
    int EraseChar = erasechar();
    int EraseWord = eraseword();
    int EraseLine = killchar();
    int count;
    int log_count;
    int literal;
    static DYN *saved;

    saved = dyn_copy(saved, bfr);
    if (logging)
	dyn_init(&history, 1);

    FieldLen = field_len;
    Prefix = pref;
    wrap = new_line;
    Imode = 1;
    errs = 0;
    bbase = CurIns = bfr;
    shift = 0;

    if ((Z = win) != 0) {
	getyx(Z, y_rawgets, x_rawgets);		/* get my initial position */
	ShowPrefix();
	(void) wmove(Z, y_rawgets, x_rawgets);
	xlast = x_rawgets + FieldLen;
	if (xlast > wMaxX(Z))
	    xlast = wMaxX(Z);

	MoveTo(bfr + strlen(bfr));
	if (wrap)
	    (void) wclrtobot(Z);

	if (Imode != first_mode)
	    ToggleMode();
	else
	    ShowAll();

    } else {
	x_rawgets =
	    y_rawgets = 0;
	xlast = wMaxX(Z);
    }

    /* set editing-position to initial column */
    if ((count = (int) strlen(CurIns)) < first_col)
	first_col = (int) count;
    CurIns += first_col;
    MoveTo(CurIns);

    for (;;) {
	if (errs) {
	    errs = 0;
	    beep();
	}

	if (command && *command && **command) {
	    log_count = FALSE;
	    if (Imode || !isdigit(UCH(**command)))
		count = 1;
	    else {
		char *s = *command;
		count = 0;
		while (isdigit(UCH(*s))) {
		    log_count = TRUE;
		    count = (count * 10) + (*s++ - '0');
		}
		*command = s;
	    }
	    on_winch(Redisplay);
	    c = decode_logch(command, (int *) 0);
	    if ((literal = to_literal(c)) == TRUE)
		c = decode_logch(command, (int *) 0);
	    on_winch((void (*)(void)) 0);
	} else {
	    if (Z)
		(void) wrefresh(Z);
	    count = 1;
	    on_winch(Redisplay);
	    c = cmdch(Imode ? (int *) 0 : &count);
	    on_winch((void (*)(void)) 0);
	    log_count = (count != 1);

	    if ((literal = to_literal(c)) == TRUE)
		c = wgetch(Z);
	}
	if (c == EOS)
	    continue;

	/*
	 * Note: the command-script will be logged exactly only if
	 * none of the repeat-counts have leading zeroes.
	 */
	if (logging) {
	    char temp[20];
	    if (literal) {
		encode_logch(temp, log_count ? &count : (int *) 0, CTL('V'));
		history = dyn_append(history, temp);
		log_count = FALSE;
	    }
	    encode_logch(temp, log_count ? &count : (int *) 0, c);
	    history = dyn_append(history, temp);
	}
#ifndef	NO_XTERM_MOUSE
	/*
	 * Use the mouse for (re)positioning the cursor within the
	 * buffer.
	 */
	if (c == KEY_MOUSE) {
	    if (xt_mouse.released) {
		if (xt_mouse.button == 1) {
		    CurIns = MoveFrom(xt_mouse.row,
				      xt_mouse.col);
		} else {
		    errs++;
		}
	    }
	    continue;
	}
#endif
	/*
	 * We return only one of three types of thing:
	 *      up/down arrow,
	 *      kill-character
	 *      or return/newline
	 * so that we can interlock this with a history-mechanism.
	 */
	if (!literal) {
	    if (c == '\r')
		c = '\n';
	    if (c == '\n') {
		MoveTo(bbase + strlen(bbase));
		if (Z && new_line)
		    (void) waddch(Z, '\n');
		break;
	    }
	    if (to_up(c)) {
		c = KEY_UP;
		break;
	    }
	    if (to_down(c)) {
		c = KEY_DOWN;
		break;
	    }

	    if (to_toggle(c)) {
		ToggleMode();
		continue;
	    }
	    if (!Imode) {
		if ((c == fast_q)
		    || ((fast_q != EOS) && (c == 'q'))) {
		    (void) strcpy(bfr, dyn_string(saved));
		    break;
		}
	    }

	}

	/*
	 * Normally we insert/edit only printing characters.
	 * In literal-mode, we can insert any ascii character.
	 */
	if (literal || (Imode && !is_special(c) && isprint(c))) {
	    while (count-- > 0) {
		if (CurIns - bfr < buffer_len)
		    InsertAt(CurIns++, c);
		else {
		    errs++;
		    break;
		}
	    }
	    continue;
	}

	if ((c == EraseChar) || (c == '\b')) {
	    CurIns = DeleteBefore(CurIns, (int) count);
	} else if (c == EraseWord) {
	    CurIns = DeleteWordBefore(CurIns, (int) count);
	} else if (c == EraseLine) {
	    if (Imode) {
		count = (int) strlen(bfr);
		(void) DeleteBefore(bfr + count, (int) count);
		break;
	    } else
		CurIns = DeleteBefore(CurIns, (int) (CurIns - bfr));

	} else if (to_left(c)) {
	    if (CurIns > bfr) {
		char *s = CurIns;
		while (count-- > 0)
		    if (--s == bfr)
			break;
		MoveTo(CurIns = s);
	    } else
		errs++;
	} else if (to_right(c)) {
	    if (*CurIns != EOS) {
		char *s = CurIns;
		while (count-- > 0)
		    if (*(++s) == EOS)
			break;
		MoveTo(CurIns = s);
	    } else
		errs++;
	} else if (to_home(c)) {
	    MoveTo(CurIns = bbase);
	} else if (to_end(c)) {
	    MoveTo(CurIns = bbase + strlen(bbase));
	} else
	    errs++;

    }

    if (Z) {
	if (!wrap && !Imode)
	    ToggleMode();

	if (!command || !*command)
	    (void) wrefresh(Z);
    }

    return (c);			/* returns character which terminated this call */
}

/*
 * Returns the logging history from the last call on 'wrawgets()'
 */
char *
rawgets_log(void)
{
    return dyn_string(history);
}

/************************************************************************
 *	test procedure							*
 ************************************************************************/
#ifdef	TEST
_MAIN
{
    int j = 0;
    int w_flag = ((argc > 1) && !strcmp(argv[1], "-w"));
    char bfr[BUFSIZ];
    static char *pref[] =
    {"^ ", "> "};

    initscr();
    rawterm();
    *bfr = EOS;
    while (strlen(bfr) < 3 * COLS) {
	(void) strcat(bfr, "abcdefghijklmnopqrstuvwxyz.");
	(void) sprintf(bfr + strlen(bfr), "%d ", j++);
    }
    move(0, 0);
    printw("You will be prompted at each line, until the buffer is empty");
    j = 1;
    for (;;) {
	move(j, 0);
	clrtobot();
	move(j, 0);
	printw("%05d> ", j);
	rawgets(bfr, pref, sizeof(bfr),
		COLS / 2, strlen(bfr), TRUE,
		w_flag, 'q',
		(char **) 0, FALSE);
	if (!*bfr)
	    break;
	if (++j >= LINES)
	    j = 1;
    }
    endwin();
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif /* TEST */
