/*
 * Title:	cmdch.c (command-character decoder)
 * Author:	T.E.Dickey
 * Created:	01 Dec 1987 (broke out of 'ded.c')
 * Modified:
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

MODULE_ID("$Id: cmdch.c,v 12.22 1996/09/25 14:30:28 tom Exp $")

#define	ESC(c)	((c) == '\033')
#define	END(s)	s[strlen(s)-1]
#define	if_C(c)	if (i_blk[j] == c)
#define	EQL(s)	(!strcmp(i_blk,((s)?(s):"")))

#if !NO_XTERM_MOUSE

#define XtermPos() (getch() - 041)	/* 0..COLS-1 or 0..LINES-1 */

XtermMouse xt_mouse;	/* state of XTerm-mouse */

#if !defined(NCURSES_MOUSE_VERSION)
static	int	double_click (_AR0)
{
#if HAVE_GETTIMEOFDAY
	static	struct	timeval	last_time;
	auto	struct	timeval	this_time;
	auto	struct	timezone this_zone;
	register int	event = FALSE;
	register long	diff;

	(void) gettimeofday(&this_time, &this_zone);

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
#endif	/* !NCURSES_MOUSE_VERSION */
#endif	/* !NO_XTERM_MOUSE */

int	cmdch(
	_AR1(int *,	cnt_))
	_DCL(int *,	cnt_)
{
	register j = 0;
	auto	int	c	= EOS,
			done	= FALSE,
			had_c	= 0,
			count	= 0;
	auto	char	i_blk[1024];
	static	int	init	= FALSE;
#if !HAVE_KEYPAD
# if !HAVE_TCAP_CURSOR
	static	char	*KU, *KD, *KR, *KL;
# endif
	static	int	ansi	= FALSE;
#endif	/* HAVE_KEYPAD */
#if NCURSES_MOUSE_VERSION && !NO_XTERM_MOUSE
	MEVENT	myevent;
#endif

	if (!init) {
		init = TRUE;
#if !HAVE_KEYPAD
# if !HAVE_TCAP_CURSOR
		{
		static	char	o_blk[1024], *a_ = o_blk;
			if (tgetent(i_blk,getenv("TERM")) <= 0)
				failed("cmdch/tgetent");
			KD = tgetstr("kd", &a_);
			KU = tgetstr("ku", &a_);
			KR = tgetstr("kr", &a_);
			KL = tgetstr("kl", &a_);
		}
# endif
		if (KD && KU && KR && KL) {
			if (ESC(*KD) && ESC(*KU) && ESC(*KR) && ESC(*KL))
				ansi	=  END(KU) == 'A'
					&& END(KD) == 'B'
					&& END(KR) == 'C'
					&& END(KL) == 'D';
		}
#endif	/* HAVE_KEYPAD */
	}

	while (!done) {

		c = getch();
#if HAVE_KEYPAD
		switch (c) {
		/*
		 * These definitions simplify the task of porting between BSD-
		 * and SYS5-curses.  Just because I turn on the keypad, I
		 * shouldn't have to worry about normal keycodes shifting about
		 * as a side-effect.
		 */
#ifdef KEY_DC
#if KEY_DC != '\177'
		case KEY_DC:	c = '\177';	done = TRUE;	break;
#endif
#endif
#ifdef KEY_BACKSPACE
#if KEY_BACKSPACE != '\b'
		case KEY_BACKSPACE: c = '\b';	done = TRUE;	break;
#endif
#endif
#ifdef KEY_ENTER
#if KEY_ENTER != '\n'
		case KEY_ENTER: c = '\n';	done = TRUE;	break;
#endif
#endif
#if NCURSES_MOUSE_VERSION && !NO_XTERM_MOUSE
		case KEY_MOUSE:
			getmouse(&myevent);
			xt_mouse.col = myevent.x;
			xt_mouse.row = myevent.y;
			if (myevent.bstate & BUTTON1_CLICKED) {
				xt_mouse.button = 1;
				xt_mouse.dbl_clik = FALSE;
			} else
			if (myevent.bstate & BUTTON1_DOUBLE_CLICKED) {
				xt_mouse.button = 1;
				xt_mouse.dbl_clik = TRUE;
			} else
			if (myevent.bstate & BUTTON2_CLICKED) {
				xt_mouse.button = 2;
				xt_mouse.dbl_clik = FALSE;
			} else
			if (myevent.bstate & BUTTON2_DOUBLE_CLICKED) {
				xt_mouse.button = 2;
				xt_mouse.dbl_clik = TRUE;
			} else
			if (myevent.bstate & BUTTON3_CLICKED) {
				xt_mouse.button = 3;
				xt_mouse.dbl_clik = FALSE;
			} else
			if (myevent.bstate & BUTTON3_DOUBLE_CLICKED) {
				xt_mouse.button = 3;
				xt_mouse.dbl_clik = TRUE;
			} else
				break;
			xt_mouse.pressed  = TRUE;
			xt_mouse.released = TRUE;
			done = TRUE;
			break;
#endif
		case KEY_UP:			/* FALLTHRU */
		case KEY_DOWN:			/* FALLTHRU */
		case KEY_LEFT:			/* FALLTHRU */
		case KEY_RIGHT:			done = TRUE;	break;
		}
		if (done)
			break;
#endif /* HAVE_KEYPAD */
		if (iscntrl(c) || j != 0)
			i_blk[j++] = c;

		if (ESC(c)) {	/* assume "standard" escapes */
			do {
				i_blk[j++] = c = getch();
			} while (!isalpha(c));
		}
		if (j) {
			i_blk[j] = EOS;
			done	= TRUE;
#if !HAVE_KEYPAD
			if	(EQL(KU))	c = KEY_UP;
			else if	(EQL(KD))	c = KEY_DOWN;
			else if	(EQL(KL))	c = KEY_LEFT;
			else if	(EQL(KR))	c = KEY_RIGHT;
			else
#endif /* !HAVE_KEYPAD */
			if (j > 1) {	/* extended escapes */
#if !NO_XTERM_MOUSE && !NCURSES_MOUSE_VERSION
				/* patch: should test for xterm_mouse */
				if (!strncmp(i_blk, "\033[M", 3)) {
					auto	int	the_button;
					static	int	last_row = -1;
					static	int	last_col = -1;

					c = KEY_MOUSE;
					the_button   = (getch() & 3) + 1;
					xt_mouse.col = XtermPos();
					xt_mouse.row = XtermPos();
					if (the_button > 3) {
						xt_mouse.released = TRUE;
					} else {
						xt_mouse.pressed  = TRUE;
						xt_mouse.released = FALSE;
						xt_mouse.button   = the_button;
						xt_mouse.dbl_clik = double_click()
							&& (last_row == xt_mouse.row)
							&& (last_col == xt_mouse.col);
						last_row = xt_mouse.row;
						last_col = xt_mouse.col;
					}
				} else
#endif	/* !NO_XTERM_MOUSE */
#if !HAVE_KEYPAD
				if (ansi) {
					j--;
					if_C('A')	c = KEY_UP;
					else if_C('B')	c = KEY_DOWN;
					else if_C('C')	c = KEY_RIGHT;
					else if_C('D')	c = KEY_LEFT;
					else {
						if (END(i_blk) != 'O'
					     	&&  END(i_blk) != '[') {
							j = 0;
							beep();
						} else
							j++; /* cannot skip */
						done = FALSE;
					}
				} else
#endif	/* !HAVE_KEYPAD */
				{
					beep();
					done = FALSE;
				}
			}
		} else if ((cnt_ != 0) && isdigit(c)) {
			had_c++;
			count = (count * 10) + (c - '0');
		} else
			done = TRUE;
	}

	if (cnt_)
		*cnt_ = (had_c != 0) ? count : 1;

	if (c == '\r')
		c = '\n';
	return(c);
}
