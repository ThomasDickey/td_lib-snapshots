/*
 * Title:	rawterm.c (set terminal to raw mode)
 * Author:	T.E.Dickey
 * Created:	24 Nov 1987
 * Modified:
 *		26 Dec 2019, provide for wheel-mouse in ncurses6
 *		09 Jul 2010, add xt_enabled flag to control whether the mouse
 *			     is used.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		16 Feb 1998, workaround: SVr4 curses doesn't do smkx unless it
 *			     had endwin() call.
 *		16 Dec 1995, integration with ncurses mouse-support; moved
 *			     'keypad()' call from 'cmdch()'.
 *		16 Jul 1994, made 'dumptty()' a library procedure.
 *		21 May 1994, ClarkNet's Solaris doesn't do savetty/resetty ok.
 *		26 Apr 1994, port to Linux
 *		23 Nov 1993, check environment variable TERM for "xterm".
 *		18 Nov 1993, added entrypoint 'cookterm()' to allow xterm
 *			     mouse control via this interface.
 *		01 Nov 1993, 'nonl()' doesn't work on HP/UX. Workaround with
 *			     gtty/stty calls.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		03 Aug 1988, use 'crmode()' rather than 'raw()' so we can see
 *			     signals in caller.
 *		28 Jul 1988, added 'nl()' to items affected.
 *
 * Function:	Set terminal to single-character mode and restore it to
 *		the normal, buffered mode.
 */

#define STR_PTYPES		/* <string.h> */
#define TRM_PTYPES		/* <termios.h> */
#include	"td_curse.h"

MODULE_ID("$Id: rawterm.c,v 12.30 2025/01/06 23:50:50 tom Exp $")

TermioT original_tty;
TermioT modified_tty;

#ifdef	TEST
void
show_term(char *s)
{
    static FILE *log;
    if (log == 0)
	log = fopen("rawterm.log", "w");
    if (log != 0) {
	dumptty(log, s);
    }
}
#else
#define show_term(s)
#endif

#if	!defined(NO_XTERM_MOUSE)
#if	!defined(NCURSES_MOUSE_VERSION)
static int
xterm_mouse(void)
{
    static int initialized;
    static int use_mouse;

    if (!initialized) {
	char *name;
	initialized = TRUE;
	if ((name = getenv("TERM")) != 0) {
	    int len = (int) strlen(name);
	    if (!strncmp(name, "xterm", 5)
		|| (len > 5 && !strcmp(name + len - 5, "xterm")))
		use_mouse = TRUE;
	}
    }
    return use_mouse;
}
#endif /* !NCURSES_MOUSE_VERSION */

#define XTERM_ENABLE_TRACKING   "\033[?1000h"	/* normal tracking mode */
#define XTERM_DISABLE_TRACKING  "\033[?1000l"

#define	Puts(s) fputs(s, stdout); (void)fflush(stdout)

static void
enable_mouse(void)
{
    if (xt_enabled) {
#if defined(NCURSES_MOUSE_VERSION)
	(void) mousemask(
			    BUTTON1_CLICKED | BUTTON1_DOUBLE_CLICKED
			    | BUTTON2_CLICKED | BUTTON2_DOUBLE_CLICKED
			    | BUTTON3_CLICKED | BUTTON3_DOUBLE_CLICKED
#if NCURSES_MOUSE_VERSION > 1
			    | BUTTON4_PRESSED
			    | BUTTON5_PRESSED
#endif
			    ,(mmask_t *) 0);
#else
	if (xterm_mouse()) {
	    Puts(XTERM_ENABLE_TRACKING);
	}
#endif
    }
}

static void
disable_mouse(void)
{
    if (xt_enabled) {
#if defined(NCURSES_MOUSE_VERSION)
	(void) mousemask((mmask_t) 0, (mmask_t *) 0);
#else
	if (xterm_mouse()) {
	    Puts(XTERM_DISABLE_TRACKING);
	}
#endif
    }
}
#else
#define	enable_mouse()
#define	disable_mouse()
#endif

/*
 * Ncurses emits rmkx/smkx when turning keypad() off/on.  SVr4 curses doesn't
 * do that, preferring endwin/refresh, which we don't want, since that clears
 * the screen as well.
 */
#if	defined(HAVE_TIGETSTR) && HAVE_PUTP && !defined(NCURSES_VERSION)
static void
set_cursor_mode(void)
{
    char *s = tigetstr("smkx");
    if (s != 0)
	putp(s);
}
#else
#define	set_cursor_mode()
#endif

/*
 * Call this to save the original terminal state, _before_ calling 'initscr()'.
 * I found this necessary on Solaris (explorer@clarknet.com) because the
 * terminal characteristics were not being saved/restored properly.  Because
 * this wasn't the first problem I'd had with SysV, I encapsulated it...
 */
void
save_terminal(void)
{
    show_term("save-terminal");
    GetTerminal(&original_tty);
}

void
restore_terminal(void)
{
    show_term("restore-terminal");
    SetTerminal(&original_tty);
}

/*
 * Set terminal to single-character mode
 */
void
rawterm(void)
{
    static int initialized;

#if defined(HAVE_INTRFLUSH)
    intrflush(stdscr, FALSE);
#endif
#if defined(HAVE_KEYPAD)
    keypad(stdscr, TRUE);
#endif
    show_term("before-raw-");
#if	defined(HAVE_CBREAK)
    cbreak();
#else
    crmode();
#endif
    noecho();
    nonl();

#ifdef __hpux			/* bug-fix */
    /* HP/UX didn't disable echo; this is a quick hack to fix that */
    {
	TermioT sb;
	GetTerminal(&sb);
	sb.c_lflag &= ~ECHO;
	SetTerminal(&sb);
    }
#endif

    if (!initialized) {
	GetTerminal(&modified_tty);
	initialized = TRUE;
    } else {
	SetTerminal(&modified_tty);
    }

    enable_mouse();
    set_cursor_mode();
    show_term("after--raw-");
}

/*
 * Set terminal back to out-of-curses state
 */
void
cookterm(void)
{
    show_term("before-cook");
    refresh();
    disable_mouse();
    restore_terminal();		/* replaces 'resetty()' */
    show_term("after--cook");
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
