#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: rawterm.c,v 12.12 1994/05/24 00:50:05 tom Exp $";
#endif

/*
 * Title:	rawterm.c (set terminal to raw mode)
 * Author:	T.E.Dickey
 * Created:	24 Nov 1987
 * Modified:
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

#define STR_PTYPES
#include	"td_curse.h"

#if defined(__hpux)
# undef HAVE_TERMIOS_H	/* patch: I should have used <termios.h> */
#endif

#if HAVE_TERMIOS_H
#  include <termios.h>
#  define SGTTY struct termios
#  if HAVE_TCGETATTR
#    define GetTerminal(p) tcgetattr(0, p)
#    define SetTerminal(p) tcsetattr(0, TCSAFLUSH, p)
#  else
#    define GetTerminal(p) ioctl(0, TCGETA, p)
#    define SetTerminal(p) ioctl(0, TCSETAF, p)
#  endif
#else
#  if HAVE_SGTTY_H
#    include <sgtty.h>
#    define SGTTY struct sgttyb
#    define GetTerminal gtty(0, p)
#    define SetTerminal stty(0, p)
#  endif
#endif

SGTTY	original_tty;
SGTTY	modified_tty;

#ifdef	TEST
void	show_term(s)
	char	*s;
{
	static	FILE	*log;
	if (log == 0)
		log = fopen("rawterm.log", "w");
	if (log != 0) {
#if HAVE_TERMIOS_H	/* we've got <termios.h> */
		int	n;
		SGTTY sb;
		GetTerminal(&sb);
		fprintf(log, "%s: \n", s);
		fprintf(log, "\tiflag %#lo\n", sb.c_iflag);
		fprintf(log, "\toflag %#lo\n", sb.c_oflag);
		fprintf(log, "\tcflag %#lo\n", sb.c_cflag);
		fprintf(log, "\tlflag %#lo\n", sb.c_lflag);
		fprintf(log, "\tchars\n");
		for (n = 0; n < NCCS; n++)	/* control chars */
			if (sb.c_cc[n] != 0)
				fprintf(log, "\t\t%d: %#o\n", n, sb.c_cc[n]);
		fprintf(log, "\n");
#else
		SGTTY sb;
		GetTerminal(&sb);
		fprintf(log,
			"%s: speed %#o/%#o erase %#o kill %#o flags %#o\n", s,
			sb.sg_ispeed,		/* input speed */
			sb.sg_ospeed,		/* output speed */
			sb.sg_erase,		/* erase character */
			sb.sg_kill,		/* kill character */
			sb.sg_flags);		/* mode flags */
#endif
		fflush(log);
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
#ifdef	SYS5_CURSES
	cbreak();
#else
	crmode();
#endif
	noecho();
	nonl();

#ifdef __hpux
	/* HP/UX didn't disable echo; this is a quick hack to fix that */
	{
		SGTTY sb;
		GetTerminal(&sb);
		sb.sg_flags &= ~ECHO;
		SetTerminal(0, &sb);
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
