/*
 * Title:	rawterm.c (set terminal to raw mode)
 * Author:	T.E.Dickey
 * Created:	24 Nov 1987
 * Modified:
 *		16 Jul 1994, made 'dumptty()' a library procedure.
 *		21 May 1994, ClarkNet's Solaris doesn't do savetty/resetty ok.
 *		26 Apr 1994, port to Linux
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
 * Function:	Set terminal to single-character mode and restore it to
 *		the normal, buffered mode.
 */

#define STR_PTYPES	/* <string.h> */
#define TRM_PTYPES	/* <termios.h> */
#include	"td_curse.h"

MODULE_ID("$Id: rawterm.c,v 12.18 1994/12/16 14:15:22 tom Exp $")

TermioT	original_tty;
TermioT	modified_tty;

#ifdef	TEST
void	show_term(
	_AR1(char *,	s))
	_DCL(char *,	s)
{
	static	FILE	*log;
	if (log == 0)
		log = fopen("rawterm.log", "w");
	if (log != 0) {
		dumptty(log, s);
	}
}
#else
#define show_term(s)
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
	if (xterm_mouse()) {
		Puts(XTERM_ENABLE_TRACKING);
	}
}

static	void	disable_mouse(_AR0)
{
	if (xterm_mouse()) {
		Puts(XTERM_DISABLE_TRACKING);
	}
}
#endif

/*
 * Call this to save the original terminal state, _before_ calling 'initscr()'.
 * I found this necessary on Solaris (explorer@clarknet.com) because the
 * terminal characteristics were not being saved/restored properly.  Because
 * this wasn't the first problem I'd had with SysV, I encapsulated it... 
 */
void	save_terminal(_AR0)
{
	show_term("save-terminal");
	GetTerminal(&original_tty);
}

void	restore_terminal(_AR0)
{
	show_term("restore-terminal");
	SetTerminal(&original_tty);
}

/*
 * Set terminal to single-character mode
 */
void	rawterm(_AR0)
{
	static	int	initialized ;

	show_term("before-raw-");
#if	HAVE_CBREAK
	cbreak();
#else
	crmode();
#endif
	noecho();
	nonl();

#ifdef __hpux	/* bug-fix */
	/* HP/UX didn't disable echo; this is a quick hack to fix that */
	{
		TermioT sb;
		GetTerminal(&sb);
		sb.c_lflag &= ~ECHO;
		SetTerminal(&sb);
	}
#endif

	if (!initialized) {
		GetTerminal(&modified_tty) ;
		initialized = TRUE ;
	} else {
		SetTerminal(&modified_tty) ;
	}

	enable_mouse();
	show_term("after--raw-");
}

/*
 * Set terminal back to out-of-curses state
 */
void	cookterm(_AR0)
{
	show_term("before-cook");
	refresh();
	disable_mouse();
	restore_terminal();	/* replaces 'resetty()' */
	show_term("after--cook");
}
