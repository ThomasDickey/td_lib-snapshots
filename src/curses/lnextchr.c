/*
 * Title:	lnextchar.c (return literal-next char value)
 * Author:	T.E.Dickey
 * Created:	20 Oct 1995
 * Modified:
 */

#define  TRM_PTYPES
#include "td_curse.h"

MODULE_ID("$Id: lnextchr.c,v 12.1 1995/10/20 10:54:30 tom Exp $")

#define	STDIN_FD 0

int	lnext_char(_AR0)
{
	int	code	= CTL('V');		/* default value */
#if USING_TERMIOS_H
	TermioT buf;
	if (tcgetattr(0, &buf) >= 0)
		code = buf.c_cc[VLNEXT];
#else
# if USING_TERMIO_H
	TermioT buf;
	if (ioctl(0, TCGETA, (char *)&buf) >= 0)
		code = buf.c_cc[VLNEXT];
# else
#  if USING_SGTTY_H
	struct tchars buf;
	if (ioctl(STDIN_FD, TIOCLGET, (caddr_t)&buf) >= 0)
		code = buf.t_lnextc;
#  endif
# endif
#endif
	return (code);
}
