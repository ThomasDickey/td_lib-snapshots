#ifndef	lint
static	char	sccs_id[] = "@(#)cmdch.c	1.3 88/05/09 07:59:45";
#endif	lint

/*
 * Title:	cmdch.c (command-character decoder)
 * Author:	T.E.Dickey
 * Created:	01 Dec 1987 (broke out of 'ded.c')
 * Modified:
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

#include	<curses.h>
#include	<ctype.h>
#include	"cmdch.h"
extern	char	*getenv(),
		*tgetstr();

#define	ESC(c)	((c) == '\033')
#define	END(s)	s[strlen(s)-1]
#define	if_C(c)	if (i_blk[j] == c)
#define	EQL(s)	(!strcmp(i_blk,((s)?(s):"")))

int
cmdch(cnt_)
int	*cnt_;
{
int	c,
	done	= FALSE,
	count	= 0;
char	i_blk[1024];
static
int	init	= FALSE,
	ansi	= FALSE;
static
char	*KU, *KD, *KR, *KL;

	if (!init) {
	static	char	o_blk[1024], *a_ = o_blk;
		init = TRUE;
		if (tgetent(i_blk,getenv("TERM")) <= 0)
			failed("cmdch/tgetent");
		KD = tgetstr("kd", &a_);
		KU = tgetstr("ku", &a_);
		KR = tgetstr("kr", &a_);
		KL = tgetstr("kl", &a_);
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
			i_blk[j] = '\0';
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
			count = (count * 10) + (c - '0');
		} else
			done = TRUE;
	}

	if (cnt_) {
		*cnt_ = (count != 0) ? count : 1;
	}
	return(c);
}
