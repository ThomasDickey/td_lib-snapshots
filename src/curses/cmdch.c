#ifndef	lint
static	char	sccs_id[] = "@(#)cmdch.c	1.1 87/12/02 08:47:33";
#endif	lint

/*
 * Title:	cmdch.c (command-character decoder)
 * Author:	T.E.Dickey
 * Created:	01 Dec 1987 (broke out of 'ded.c')
 *
 * Function:	Read a command-character.  If 'cnt_' is set, permit a
 *		repeat-count to be associated with the command.
 *
 *		The arrow keys are decoded into mnemonic control-keys:
 *			CTL(L) = left
 *			CTL(R) = right
 *			CTL(U) = up
 *			CTL(D) = down
 */

#include	<curses.h>
#include	<ctype.h>
extern	char	*getenv(),
		*tgetstr();

#define	CTL(c)	('c'&037)
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
int	init	= FALSE;
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
	}

	while (!done) {
	register j = 0;
		c = getch();
		if (iscntrl(c))
			i_blk[j++] = c;
		if (c == '\033') {	/* assume "standard" escapes */
			do {
				i_blk[j++] = c = getch();
			} while (!isalpha(c));
		}
		if (j) {
			i_blk[j] = '\0';
			done	= TRUE;
			if	(EQL(KU))	c = CTL(u);
			else if	(EQL(KD))	c = CTL(d);
			else if	(EQL(KL))	c = CTL(l);
			else if	(EQL(KR))	c = CTL(r);
			else if (EQL(HO))	c = 'H';
			else if (j > 1)		{ beep(); done = FALSE; }
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
