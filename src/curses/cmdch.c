#ifndef	lint
static	char	Id[] = "$Id: cmdch.c,v 8.0 1990/03/12 07:54:01 ste_cm Rel $";
#endif	lint

/*
 * Title:	cmdch.c (command-character decoder)
 * Author:	T.E.Dickey
 * Created:	01 Dec 1987 (broke out of 'ded.c')
 * $Log: cmdch.c,v $
 * Revision 8.0  1990/03/12 07:54:01  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  90/03/12  07:54:01  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  90/03/12  07:54:01  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.2  90/03/12  07:54:01  dickey
 *		lint (apollo sr10.1)
 *		
 *		Revision 5.1  90/01/30  08:26:46  dickey
 *		permit explicit zero-count to be returned.  default is still 1.
 *		
 *		Revision 5.0  89/10/04  11:32:28  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.1  89/10/04  11:32:28  dickey
 *		apollo SR10.1 curses (like sun) has KD, KU, KR and KL data.
 *		don't ask for it twice!
 *		
 *		Revision 4.0  88/08/11  07:13:14  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/08/11  07:13:14  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/11  07:13:14  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.5  88/08/11  07:13:14  dickey
 *		sccs2rcs keywords
 *		
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
#define		CUR_PTYPES
#include	"ptypes.h"
#include	<ctype.h>
#include	"cmdch.h"
extern	char	*getenv();

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

int
cmdch(cnt_)
int	*cnt_;
{
int	c,
	done	= FALSE,
	had_c	= 0,
	count	= 0;
char	i_blk[1024];
static
int	init	= FALSE,
	ansi	= FALSE;
#ifndef	HAS_CURSOR
static
char	*KU, *KD, *KR, *KL;
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
			had_c++;
			count = (count * 10) + (c - '0');
		} else
			done = TRUE;
	}

	if (cnt_) {
		*cnt_ = (had_c != 0) ? count : 1;
	}
	return(c);
}
