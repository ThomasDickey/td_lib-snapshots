#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: rawterm.c,v 12.7 1993/12/02 15:20:31 dickey Exp $";
#endif

/*
 * Title:	rawterm.c (set terminal to raw mode)
 * Author:	T.E.Dickey
 * Created:	24 Nov 1987
 * Modified:
 *		23 Nov 1993, check environment variable TERM for "xterm".
 *		18 Nov 1993, added entrypoint 'cookterm()' to allow xterm
 *			     mouse control via this interface.
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

#define STR_PTYPES
#include	"td_curse.h"

#ifdef __hpux
#include <sgtty.h>
#endif

#ifdef	NO_XTERM_MOUSE
#define	enable_mouse()
#define	disable_mouse()
#else
static	int	xterm_mouse(_AR0)
{
	static	int	initialized;
	static	int	use_mouse;

	if (!initialized) {
		char	*name;
		initialized = TRUE;
		if ((name = getenv("TERM")) != 0) {
			int	len = strlen(name);
			if (!strncmp(name, "xterm", 5)
			 || (len > 5 && !strcmp(name+len-5, "xterm")))
				use_mouse = TRUE;
		}
	}
	return use_mouse;
}

#define XTERM_ENABLE_TRACKING   "\033[?1000h"	/* normal tracking mode */
#define XTERM_DISABLE_TRACKING  "\033[?1000l"

#define	Puts(s) fputs(s, stdout); (void)fflush(stdout)

static	void	enable_mouse(_AR0)
{
	if (xterm_mouse) {
		Puts(XTERM_ENABLE_TRACKING);
	}
}

static	void	disable_mouse(_AR0)
{
	if (xterm_mouse) {
		Puts(XTERM_DISABLE_TRACKING);
	}
}
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
	enable_mouse();
}

/*
 * Set terminal back to out-of-curses state
 */
void	cookterm(_AR0)
{
	refresh();
	disable_mouse();
	resetty();
}
