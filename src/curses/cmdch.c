#ifndef	lint
static	char	Id[] = "$Id: cmdch.c,v 12.1 1993/09/21 18:54:05 dickey Exp $";
#endif

/*
 * Title:	cmdch.c (command-character decoder)
 * Author:	T.E.Dickey
 * Created:	01 Dec 1987 (broke out of 'ded.c')
 * Modified:
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
#include	"cmdch.h"
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
		extern	char	*tgetstr();
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
