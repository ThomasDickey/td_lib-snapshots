#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: rawterm.c,v 12.3 1993/11/01 14:38:10 dickey Exp $";
#endif

/*
 * Title:	rawterm.c (set terminal to raw mode)
 * Author:	T.E.Dickey
 * Created:	24 Nov 1987
 * Modified:
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
 * Function:	Set terminal to single-character mode
 */

#include	"td_curse.h"

#ifdef __hpux
#include <sgtty.h>
#endif

/*
 * Set terminal to single-character mode
 */
void	rawterm(_AR0)
{
#ifdef	SYSTEM5
	cbreak();
#else	/* SYSTEM5 */
	crmode();
#endif	/* SYSTEM5 */
	noecho();
	nonl();

#ifdef __hpux
	{
		struct	sgttyb sb;
		gtty(0, &sb);
		sb.sg_flags &= ~ECHO;
		stty(0, &sb);
	}
#endif
}
