#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: cmdch.c,v 12.5 1993/11/26 14:10:12 dickey Exp $";
#endif

/*
 * Title:	cmdch.c (command-character decoder)
 * Author:	T.E.Dickey
 * Created:	01 Dec 1987 (broke out of 'ded.c')
 * Modified:
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
 */

#define		STR_PTYPES
#include	"td_curse.h"
#include	<ctype.h>

#define	ESC(c)	((c) == '\033')
#define	END(s)	s[strlen(s)-1]
#define	if_C(c)	if (i_blk[j] == c)
#define	EQL(s)	(!strcmp(i_blk,((s)?(s):"")))

/* pre-SR10 apollo systems do not have cursor-codes in curses */
#define	HAS_CURSOR
#ifdef	apollo
#ifndef	apollo_sr10
#undef	HAS_CURSOR
#endif
#endif

#if	SYS5_CURSES
#undef	HAS_CURSOR	/* patch: want to use 'keypad()' */
#endif

#ifndef	NO_XTERM_MOUSE
#include <sys/time.h>

#define XtermPos() (getch() - 041)	/* 0..COLS-1 or 0..LINES-1 */

static	int	double_click (_AR0)
{
	static	struct	timeval	last_time;
	auto	struct	timeval	this_time;
	auto	struct	timezone this_zone;
	register int	event = FALSE;
	register long	diff;

	(void) gettimeofday(&this_time, this_zone);

	diff = this_time.tv_sec - last_time.tv_sec;
	if (diff <= 1) {
		diff *= 1000000L;
		diff += (this_time.tv_usec - last_time.tv_usec);
		event = diff < 1000000L;
	}

	last_time = this_time;
	return event;
}
#endif

int	cmdch(
	_AR1(int *,	cnt_))
	_DCL(int *,	cnt_)
{
	auto	int	c	= EOS,
			done	= FALSE,
			had_c	= 0,
			count	= 0;
	auto	char	i_blk[1024];
	static	int	init	= FALSE,
			ansi	= FALSE;
#ifndef	HAS_CURSOR
	static	char	*KU, *KD, *KR, *KL;
#endif

	if (!init) {
		init = TRUE;
#ifndef	HAS_CURSOR
		{
		static	char	o_blk[1024], *a_ = o_blk;
			if (tgetent(i_blk,getenv("TERM")) <= 0)
				failed("cmdch/tgetent");
			KD = tgetstr("kd", &a_);
			KU = tgetstr("ku", &a_);
			KR = tgetstr("kr", &a_);
			KL = tgetstr("kl", &a_);
		}
#endif
		if (KD && KU && KR && KL) {
			if (ESC(*KD) && ESC(*KU) && ESC(*KR) && ESC(*KL))
				ansi	=  END(KU) == 'A'
					&& END(KD) == 'B'
					&& END(KR) == 'C'
					&& END(KL) == 'D';
		}
	}

	while (!done) {
		register j = 0;

		c = getch();
		if (iscntrl(c))
			i_blk[j++] = c;
		if (ESC(c)) {	/* assume "standard" escapes */
			do {
				i_blk[j++] = c = getch();
			} while (!isalpha(c));
		}
		if (j) {
			i_blk[j] = EOS;
			done	= TRUE;
			if	(EQL(KU))	c = ARO_UP;
			else if	(EQL(KD))	c = ARO_DOWN;
			else if	(EQL(KL))	c = ARO_LEFT;
			else if	(EQL(KR))	c = ARO_RIGHT;
			else if (j > 1) {	/* extended escapes */
#ifndef	NO_XTERM_MOUSE
				/* patch: should test for xterm_mouse */
				if (!strncmp(i_blk, "\033[M", 3)) {
					auto	int	the_button;
					static	int	last_row = -1;
					static	int	last_col = -1;

					c = ARO_MOUSE;
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
#endif
				if (ansi) {
					j--;
					if_C('A')	c = ARO_UP;
					else if_C('B')	c = ARO_DOWN;
					else if_C('C')	c = ARO_RIGHT;
					else if_C('D')	c = ARO_LEFT;
					else {
						beep();
						done = FALSE;
					}
				} else {
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
