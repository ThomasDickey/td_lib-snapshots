/*
 * Title:	cmdch.c (command-character decoder)
 * Author:	T.E.Dickey
 * Created:	01 Dec 1987 (broke out of 'ded.c')
 * Modified:
 *		28 Apr 2020, filter ERRs after a KEY_RESIZE.
 *		26 Dec 2019, convert wheel-mouse to up/down arrow
 *		17 Dec 2019, simplify ifdefs vs BSD-curses.
 *		29 Nov 2019, gcc warnings
 *		25 Dec 2014, coverity warnings
 *		09 Jul 2010, add xt_enabled, to control mouse-initialization.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		03 Jul 2003, check for KEY_RESIZE makes ncurses sigwinch work
 *		15 Feb 1998, add home/end/ppage/npage keys.
 *		25 Sep 1996, fix for ANSI arrow-key decoding
 *		16 Dec 1995, integration with ncurses mouse-support.
 *		04 Jul 1994, mods for autoconf.
 *		18 Nov 1993, added xt_mouse support.
 *		05 Nov 1993, absorb "cmdch.h" into "td_curse.h"
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		12 Aug 1992, map carriage-return to newline.
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		12 Mar 1990, lint (apollo sr10.1)
 *		30 Jan 1990, permit explicit zero-count to be returned.
 *			     Default is still 1.
 *		04 Oct 1989, apollo SR10.1 curses (like sun) has KD, KU, KR and
 *			     KL data.  Don't ask for it twice!
 *		09 May 1988, extended arrow-key comparison so this will match
 *			     ansi/vt100 & vt52 terminals better.
 *		27 Apr 1988, broke out "cmdch.h".
 *
 * Function:	Read a command-character.  If 'cnt_' is set, permit a
 *		repeat-count to be associated with the command.
 *
 *		The arrow keys are decoded into mnemonic control-keys
 *		(see "cmdch.h").
 *
 * Configure:
 *		HAVE_TCAP_CURSOR is true iff the curses library defines KD, KU,
 *			etc., which are attributes with arrow keys.
 *
 *		HAVE_KEYPAD is true iff the curses library supports 'keypad()',
 *			so that we can assume 'getch()' will translate arrow
 *			keys, rather than requiring us to do so.
 *
 *		NO_XTERM_MOUSE is true to suppress the xterm mouse logic, which
 *			requires a special escape sequence.
 */

#define		STR_PTYPES
#define		TIM_PTYPES
#include	"td_curse.h"
#include	<ctype.h>

MODULE_ID("$Id: cmdch.c,v 12.43 2025/01/06 23:59:56 tom Exp $")

#define	ESC(c)	((c) == '\033')
#define	END(s)	s[strlen(s)-1]
#define	CMP(c)	(i_blk[j] == c)
#define	EQL(s)	(!strcmp(i_blk,((s)?(s):"")))

#if !defined(NO_XTERM_MOUSE)

#define XtermPos() (getch() - 041)	/* 0..COLS-1 or 0..LINES-1 */

int xt_enabled = TRUE;		/* true if we accept XTerm-mouse events */
XtermMouse xt_mouse;		/* state of XTerm-mouse */

#if !(defined(HAVE_KEYPAD) || defined(NCURSES_MOUSE_VERSION))
static int
double_click(void)
{
#if defined(HAVE_GETTIMEOFDAY)
    static struct timeval last_time;
    struct timeval this_time;
    int event = FALSE;
    long diff;

    (void) gettimeofday(&this_time, NULL);

    diff = this_time.tv_sec - last_time.tv_sec;
    if (diff <= 1) {
	diff *= 1000000L;
	diff += (this_time.tv_usec - last_time.tv_usec);
	event = diff < 1000000L;
    }

    last_time = this_time;
    return event;
#else
    return FALSE;
#endif
}
#endif /* !NCURSES_MOUSE_VERSION */
#endif /* !NO_XTERM_MOUSE */

#if !defined(HAVE_KEYPAD)
# if !defined(HAVE_TCAP_CURSOR)
static char *KU, *KD, *KR, *KL;
static char *KH;
# endif
static char *kH, *KP, *KN;	/* not generally in BSD curses */
static int ansi = FALSE;

static int
known_key(char *i_blk)
{
    if (EQL(KU))
	return KEY_UP;
    else if (EQL(KD))
	return KEY_DOWN;
    else if (EQL(KL))
	return KEY_LEFT;
    else if (EQL(KR))
	return KEY_RIGHT;
    else if (EQL(KH))
	return KEY_HOME;
    else if (EQL(kH))
	return KEY_END;
    else if (EQL(KP))
	return KEY_PPAGE;
    else if (EQL(KN))
	return KEY_NPAGE;
    return EOS;
}
#endif /* HAVE_KEYPAD */

int
cmdch(int *cnt_)
{
    int j = 0;
    int c = EOS;
    int done = FALSE;
    int had_c = 0;
    int count = 0;

    static int init = FALSE;

#if !defined(HAVE_KEYPAD)
    char i_blk[1024];
#endif

#if defined(NCURSES_MOUSE_VERSION) && !defined(NO_XTERM_MOUSE)
    MEVENT myevent;
#endif

    if (!init) {
	init = TRUE;
#if !defined(HAVE_KEYPAD)
	{
	    static char o_blk[1024], *a_ = o_blk;
	    if (tgetent(i_blk, getenv("TERM")) <= 0)
		failed("cmdch/tgetent");
# if !defined(HAVE_TCAP_CURSOR)
	    KD = tgetstr("kd", &a_);
	    KU = tgetstr("ku", &a_);
	    KR = tgetstr("kr", &a_);
	    KL = tgetstr("kl", &a_);
	    KH = tgetstr("kh", &a_);
# endif
	    kH = tgetstr("kH", &a_);
	    KP = tgetstr("kP", &a_);
	    KN = tgetstr("kN", &a_);
	}
	if (KD && KU && KR && KL) {
	    if (ESC(*KD) && ESC(*KU) && ESC(*KR) && ESC(*KL))
		ansi = END(KU) == 'A'
		    && END(KD) == 'B'
		    && END(KR) == 'C'
		    && END(KL) == 'D';
	}
#endif /* HAVE_KEYPAD */
    }

    while (!done) {

	c = getch();
#if defined(HAVE_KEYPAD)
	switch (c) {
	    /*
	     * These definitions simplify the task of porting between BSD-
	     * and SYS5-curses.  Just because I turn on the keypad, I
	     * shouldn't have to worry about normal keycodes shifting about
	     * as a side-effect.
	     */
#ifdef KEY_DC
#if KEY_DC != '\177'
	case KEY_DC:
	    c = '\177';
	    done = TRUE;
	    break;
#endif
#endif
#ifdef KEY_BACKSPACE
#if KEY_BACKSPACE != '\b'
	case KEY_BACKSPACE:
	    c = '\b';
	    done = TRUE;
	    break;
#endif
#endif
#ifdef KEY_ENTER
#if KEY_ENTER != '\n'
	case KEY_ENTER:
	    c = '\n';
	    done = TRUE;
	    break;
#endif
#endif
#if defined(NCURSES_MOUSE_VERSION) && !defined(NO_XTERM_MOUSE)
	case KEY_MOUSE:
	    getmouse(&myevent);
	    xt_mouse.col = myevent.x;
	    xt_mouse.row = myevent.y;
	    if (myevent.bstate & BUTTON1_CLICKED) {
		xt_mouse.button = 1;
		xt_mouse.dbl_clik = FALSE;
	    } else if (myevent.bstate & BUTTON1_DOUBLE_CLICKED) {
		xt_mouse.button = 1;
		xt_mouse.dbl_clik = TRUE;
	    } else if (myevent.bstate & BUTTON2_CLICKED) {
		xt_mouse.button = 2;
		xt_mouse.dbl_clik = FALSE;
	    } else if (myevent.bstate & BUTTON2_DOUBLE_CLICKED) {
		xt_mouse.button = 2;
		xt_mouse.dbl_clik = TRUE;
	    } else if (myevent.bstate & BUTTON3_CLICKED) {
		xt_mouse.button = 3;
		xt_mouse.dbl_clik = FALSE;
	    } else if (myevent.bstate & BUTTON3_DOUBLE_CLICKED) {
		xt_mouse.button = 3;
		xt_mouse.dbl_clik = TRUE;
	    }
#if NCURSES_MOUSE_VERSION > 1
	    else if (myevent.bstate & BUTTON4_PRESSED) {
		c = KEY_DOWN;
		done = TRUE;
		break;
	    } else if (myevent.bstate & BUTTON5_PRESSED) {
		c = KEY_UP;
		done = TRUE;
		break;
	    }
#endif
	    else
		break;
	    xt_mouse.pressed = TRUE;
	    xt_mouse.released = TRUE;
	    done = TRUE;
	    break;
#endif
#ifdef KEY_RESIZE
	case KEY_RESIZE:
	    timeout(50);
	    {
		int retry;
		int skips;
		for (retry = skips = 0; (retry - skips) < 10; ++retry) {
		    c = getch();
		    if (c == KEY_RESIZE) {
			skips = retry;
		    } else if (c != ERR) {
			break;
		    }
		}
		if (c != ERR)
		    ungetch(c);
		c = KEY_RESIZE;
	    }
	    timeout(-1);
	    break;		/* eat this for sigwinch */
#endif
	case KEY_HOME:		/* FALLTHRU */
	case KEY_END:		/* FALLTHRU */
	case KEY_PPAGE:	/* FALLTHRU */
	case KEY_NPAGE:	/* FALLTHRU */
	case KEY_UP:		/* FALLTHRU */
	case KEY_DOWN:		/* FALLTHRU */
	case KEY_LEFT:		/* FALLTHRU */
	case KEY_RIGHT:
	    done = TRUE;
	    break;
	}
	if (done)
	    break;
#endif /* HAVE_KEYPAD */

#if !defined(HAVE_KEYPAD) && !defined(NO_XTERM_MOUSE)
	if (iscntrl(c) || j != 0)
	    i_blk[j++] = (char) c;
	if (ESC(c)) {		/* assume "standard" escapes */
	    do {
		i_blk[j++] = c = getch();
		i_blk[j] = EOS;
	    } while (!isalpha(c) && !known_key(i_blk));
	}
#endif
	if (j) {
	    done = TRUE;
#if !defined(HAVE_KEYPAD)
	    i_blk[j] = EOS;
	    if (known_key(i_blk))
		c = known_key(i_blk);
	    else
#if !defined(NO_XTERM_MOUSE)
	    if (j > 1) {	/* extended escapes */
		/* patch: should test for xterm_mouse */
		if (!strncmp(i_blk, "\033[M", 3)) {
		    int the_button;
		    static int last_row = -1;
		    static int last_col = -1;

		    c = KEY_MOUSE;
		    the_button = (getch() & 3) + 1;
		    xt_mouse.col = XtermPos();
		    xt_mouse.row = XtermPos();
		    if (the_button > 3) {
			xt_mouse.released = TRUE;
		    } else {
			xt_mouse.pressed = TRUE;
			xt_mouse.released = FALSE;
			xt_mouse.button = the_button;
			xt_mouse.dbl_clik = double_click()
			    && (last_row == xt_mouse.row)
			    && (last_col == xt_mouse.col);
			last_row = xt_mouse.row;
			last_col = xt_mouse.col;
		    }
		} else if (ansi) {
		    j--;
		    if (CMP('A'))
			c = KEY_UP;
		    else if (CMP('B'))
			c = KEY_DOWN;
		    else if (CMP('C'))
			c = KEY_RIGHT;
		    else if (CMP('D'))
			c = KEY_LEFT;
		    else {
			if (END(i_blk) != 'O'
			    && END(i_blk) != '[') {
			    j = 0;
			    beep();
			} else
			    j++;	/* cannot skip */
			done = FALSE;
		    }
		} else {
		    beep();
		    done = FALSE;
		}
	    }
#endif /* !defined(NO_XTERM_MOUSE) */
#endif /* !HAVE_KEYPAD */
	} else if ((cnt_ != NULL) && isdigit(c)) {
	    had_c++;
	    count = ((count * 10) + (c - '0'));
	} else
	    done = TRUE;
    }

    if (cnt_)
	*cnt_ = (had_c != 0) ? count : 1;

    if (c == '\r')
	c = '\n';
    return (c);
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
