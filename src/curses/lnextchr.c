/*
 * Title:	lnextchar.c (return literal-next char value)
 * Author:	T.E.Dickey
 * Created:	20 Oct 1995
 * Modified:
 */

#define  TRM_PTYPES
#include "td_curse.h"

MODULE_ID("$Id: lnextchr.c,v 12.3 2002/07/05 11:22:17 tom Exp $")

#define	STDIN_FD 0

int	lnext_char(_AR0)
{
	int	code	= CTL('V');		/* default value */
#if defined(USING_TERMIOS_H) && defined(VLNEXT)
	TermioT buf;
	if (tcgetattr(0, &buf) >= 0)
		code = buf.c_cc[VLNEXT];
#else
# if defined(USING_TERMIO_H) && defined(VLNEXT)
	TermioT buf;
	if (ioctl(0, TCGETA, (char *)&buf) >= 0)
		code = buf.c_cc[VLNEXT];
# else
#  if defined(USING_SGTTY_H)
	struct tchars buf;
	if (ioctl(STDIN_FD, TIOCLGET, (caddr_t)&buf) >= 0)
		code = buf.t_lnextc;
#  endif
# endif
#endif
	return (code);
}
