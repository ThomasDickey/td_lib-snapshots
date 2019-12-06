/* $Id: td_curse.h,v 12.65 2019/12/06 11:40:07 tom Exp $ */

/*
 * TD_LIB CURSES-related definitions
 */

#ifndef		TD_CURSE_H
#define		TD_CURSE_H

#ifndef		PTYPES_H
#include <ptypes.h>
#endif		/* PTYPES_H */

#undef ERR	/* work around glib 2.1.3 bug */

#ifdef sun	/* <termios.h> defines stuff in a different way :-( */
#undef ECHO
#undef NL0
#undef NL1
#undef TAB0
#undef TAB1
#undef TAB2
#undef XTABS
#undef CR0
#undef CR1
#undef CR2
#undef CR3
#undef FF0
#undef FF1
#undef BS0
#undef BS1
#undef TOSTOP
#undef FLUSHO
#undef PENDIN
#undef NOFLSH
#undef VINTR	/* fix for SGTTY definition in Solaris curses.h */
#endif

#ifdef HZ
#undef HZ	/* conflict with EMX's param.h */
#endif

#ifdef HAVE_NCURSES_NCURSES_H
#include	<ncurses/ncurses.h>
#else
#ifdef HAVE_NCURSES_CURSES_H
#include	<ncurses/curses.h>
#else
#ifdef HAVE_NCURSES_H
#include	<ncurses.h>
#else
#include	<curses.h>
#endif
#endif
#endif

#if defined(NEED_TERM_H)

#if defined(HAVE_NCURSESW_TERM_H)
#include	<ncursesw/term.h>
#elif defined(HAVE_NCURSES_TERM_H)
#include	<ncurses/term.h>
#elif defined(HAVE_TERM_H)
#include	<term.h>
#endif

#else

#if defined(HAVE_NCURSES_TERMCAP_H) && defined(NCURSES_VERSION)
#include <ncurses/termcap.h>
#endif

#if defined(HAVE_TERMCAP_H) && !defined(NCURSES_VERSION)
#include <termcap.h>
#endif

#endif

/*----------------------------------------------------------------------------*/
/*
 * There are a few structural differences between BSD and SYS5 curses that we
 * assume (because they're hard to test):
 *
 *	a) the stdscr and curscr windows of BSD can be directly manipulated
 *	   (e.g., for resizing and touching).
 *	b) there are different member names for the WINDOW struct (only
 *	   important when debugging).
 *	c) some implementations of 'touchwin()' do not work (these usually
 *	   are the older BSD versions).
 */
#if defined(HAVE_KEYPAD)
#define GOOD_TOUCHWIN 1
#endif

/*----------------------------------------------------------------------------*/
/*
 * The SYS5 clone "ncurses" 1.8.5 has an off-by-one error in the computation of
 * the _maxy/_maxx values.  These macros are used to hide this bug.
 */
#if  defined(HAVE_GETMAXX) && defined(HAVE_GETMAXY)
#define wMaxX(w) getmaxx(w)
#define wMaxY(w) getmaxy(w)
#else
#if  defined(HAVE_GETMAXYX)
#undef getmaxx
#undef getmaxy
extern int getmaxx(WINDOW *);
extern int getmaxy(WINDOW *);
#define wMaxX(w) getmaxx(w)
#define wMaxY(w) getmaxy(w)
#endif
#endif

#if  defined(HAVE_GETBEGX) && defined(HAVE_GETBEGY)
#define wBegX(w) getbegx(w)
#define wBegY(w) getbegy(w)
#else
#if  defined(HAVE_GETBEGYX)
#undef getbegx
#undef getbegy
extern int getbegx(WINDOW *);
extern int getbegy(WINDOW *);
#define wBegX(w) getbegx(w)
#define wBegY(w) getbegy(w)
#endif
#endif

#ifndef wMaxX
#if defined(CURSES_LIKE_BSD44)
#define	wMaxX(w)	((w)->maxx)
#define	wMaxY(w)	((w)->maxy)
#else
#define	wMaxX(w)	(((w)->_maxx) + (COLS  - stdscr->_maxx))
#define	wMaxY(w)	(((w)->_maxy) + (LINES - stdscr->_maxy))
#endif
#endif	/* wMaxX */

#ifndef wBegX
#if defined(CURSES_LIKE_BSD44)
#define	wBegX(w)	((w)->begx)
#define	wBegY(w)	((w)->begy)
#else
#define	wBegX(w)	((w)->_begx)
#define	wBegY(w)	((w)->_begy)
#endif
#endif

/*----------------------------------------------------------------------------*/
#if defined(CURSES_LIKE_BSD)
#define CursesLine(win,y)	(win)->_y[y]
#define CursesFirstCh(win,y)	(win)->_firstch[y]
#define CursesLastCh(win,y)	(win)->_lastch[y]
#endif
#if defined(CURSES_LIKE_BSD44)	/* curses with SlackWare 2.2 (bsd 4.4) */
#define CursesType              __LDATA
#define CursesData(win,y,x)	(win)->lines[y]->line[x].ch
#define CursesLine(win,y)	(win)->lines[y]->line
#define CursesFirstCh(win,y)	(win)->lines[y]->firstch
#define CursesLastCh(win,y)	(win)->lines[y]->lastch
#include <termios.h>
#endif
#if defined(CURSES_LIKE_SYSV)
#define CursesLine(win,y)	(win)->_line[y]
#define CursesFirstCh(win,y)	(win)->_firstchar[y]
#define CursesLastCh(win,y)	(win)->_lastchar[y]
#endif
#if defined(CURSES_LIKE_NCURSES) /* ncurses 1.8.8 and later */
#define CursesLine(win,y)	(win)->_line[y].text
#define CursesFirstCh(win,y)	(win)->_line[y].firstchar
#define CursesLastCh(win,y)	(win)->_line[y].lastchar
#endif

#ifndef ERR
#define ERR (-1)
#endif

#ifndef OK
#if ERR < 0
#define OK  (0)
#else
#define OK  (!(ERR))
#endif
#endif

#ifndef CursesType
#define CursesType chtype
#endif

#ifndef CursesData
#define CursesData(win,y,x) CursesLine(win,y)[x]
#endif

/*----------------------------------------------------------------------------*/
#ifdef MSDOS
#define NO_XTERM_MOUSE
#endif

/*
 * definitions for 'cmdch()'
 */
#define	CTL(c)		(c&037)
#define	ARO(c)		(CTL(c)|0200)

#ifndef KEY_UP
#define KEY_UP          ARO('U')
#endif
#ifndef KEY_DOWN
#define KEY_DOWN        ARO('D')
#endif
#ifndef KEY_LEFT
#define KEY_LEFT        ARO('L')
#endif
#ifndef KEY_RIGHT
#define KEY_RIGHT       ARO('R')
#endif

#ifndef KEY_HOME
#define KEY_HOME        ARO('H')
#endif
#ifndef KEY_END
#define KEY_END         ARO('E')
#endif
#ifndef KEY_PPAGE
#define KEY_PPAGE       ARO('P')
#endif
#ifndef KEY_NPAGE
#define KEY_NPAGE       ARO('N')
#endif

#ifndef KEY_MOUSE
#define KEY_MOUSE       ARO('M')
#endif

/*
 * note: System5 curses does not define the 'screen' structure
 */
#if !defined(HAVE_TYPE_CHTYPE)
	typedef	char	chtype;		/* sys5-curses data-type */
#endif

#if	defined(lint) && NEED_STRUCT_SCREEN
	struct	screen	{ int dummy; };
#endif

#ifndef beep
#if defined(HAVE_BEEP) && defined(NEED_BEEP)
extern	int	beep		(void);
#endif /* HAVE_BEEP */
#endif

	/* sysv has, but bsd doesn't */
#if !defined(HAVE_UNGETCH)
#define ungetch(c) ungetc(c,stdin)
#endif

#if defined(HAVE_ENDWIN) && defined(NEED_ENDWIN)
extern	int	endwin		(void);
#endif /* HAVE_ENDWIN */
#if defined(HAVE_PRINTW) && defined(NEED_PRINTW)
extern	int	printw		(char *f, ...);
#endif /* HAVE_PRINTW */
#if defined(USING_SGTTY_H)
#if defined(HAVE_STTY) && defined(NEED_STTY)
extern	int	stty		(int f, struct sgttyb *p);
#endif /* HAVE_STTY */
#endif
#if defined(HAVE_TGETENT) && defined(NEED_TGETENT)
extern	int	tgetent		(char *p, const char *t);
#endif /* HAVE_TGETENT */
#if defined(HAVE_TGETNUM) && defined(NEED_TGETNUM)
extern	int	tgetnum		(const char *n);
#endif /* HAVE_TGETNUM */
#if defined(HAVE_TGETSTR) && defined(NEED_TGETSTR)
extern	char *	tgetstr		(const char *n, char **p);
#endif /* HAVE_TGETSTR */
#if defined(HAVE_TOUCHWIN) && defined(NEED_TOUCHWIN)
extern	int	touchwin	(WINDOW *w);
#endif /* HAVE_TOUCHWIN */
#if defined(HAVE_WADDCH) && defined(NEED_WADDCH)
extern	int	waddch		(WINDOW *w, chtype c);
#endif /* HAVE_WADDCH */
#if defined(HAVE_WADDSTR) && defined(NEED_WADDSTR)
extern	int	waddstr		(WINDOW *w, char *s);
#endif /* HAVE_WADDSTR */
#if defined(HAVE_WCLRTOBOT) && defined(NEED_WCLRTOBOT)
extern	int	wclrtobot	(WINDOW *w);
#endif /* HAVE_WCLRTOBOT */
#if defined(HAVE_WCLRTOEOL) && defined(NEED_WCLRTOEOL)
extern	int	wclrtoeol	(WINDOW *w);
#endif /* HAVE_WCLRTOEOL */
#if defined(HAVE_WGETCH) && defined(NEED_WGETCH)
extern	int	wgetch		(WINDOW *w);
#endif /* HAVE_WGETCH */
#if defined(HAVE_WMOVE) && defined(NEED_WMOVE)
extern	int	wmove		(WINDOW *w, int ybase, int xbase);
#endif /* HAVE_WMOVE */
#if defined(HAVE_WREFRESH) && defined(NEED_WREFRESH)
extern	int	wrefresh	(WINDOW *w);
#endif /* HAVE_WREFRESH */
#if defined(HAVE_WSTANDEND) && defined(NEED_WSTANDEND)
extern	int	wstandend	(WINDOW *w);
#endif /* HAVE_WSTANDEND */
#if defined(HAVE_WSTANDOUT) && defined(NEED_WSTANDOUT)
extern	int	wstandout	(WINDOW *w);
#endif /* HAVE_WSTANDOUT */

#ifdef	lint
#undef	raw
#define raw()		/* empty */
#undef	noraw
#define noraw()		/* empty */
#undef	cbreak
#define cbreak()	/* empty */
#undef	nocbreak
#define nocbreak()	/* empty */
#undef	crmode
#define crmode()	/* empty */
#undef	nocrmode
#define nocrmode()	/* empty */
#undef	echo
#define echo()		/* empty */
#undef	noecho
#define noecho()	/* empty */
#undef	nl
#define nl()		/* empty */
#undef	nonl
#define nonl()		/* empty */
#undef	savetty
#define	savetty()	/* empty */
#undef	resetty
#define	resetty()	/* empty */
#endif

#if defined(NCURSES_VERSION) && defined(HAVE__NC_FREE_AND_EXIT)
/* nc_alloc.h normally not installed */
extern void _nc_free_and_exit(int) GCC_NORETURN;
#define ExitProgram(code) _nc_free_and_exit(code)
#endif

#ifndef ExitProgram
#define ExitProgram(code) exit(code)
#endif

#if defined(LINTLIBRARY)
#define extern	/* nothing */
#endif
	/* addchnst.c ------------------------------------------------- */
#if	!defined(HAVE_ADDCHNSTR) && !defined(TESTING_CONFIG_H)
	int	waddchnstr(
			WINDOW *	w,
			chtype *	s,
			int	n
				)
			;
#define addchnstr(str,n)	waddchnstr(stdscr,str, n)
#endif

	/* beep.c ----------------------------------------------------- */
#if	!defined(HAVE_BEEP) && !defined(TESTING_CONFIG_H)
	void	beep(void)
			;
#endif

	/* cmdch.c ---------------------------------------------------- */
#ifndef	NO_XTERM_MOUSE
typedef	struct	{
	int	col;		/* 0 .. COLS-1 */
	int	row;		/* 0 .. LINES-1 */
	int	button;		/* 1, 2 or 3 */
	int	pressed;	/* boolean */
	int	released;	/* boolean */
	int	dbl_clik;	/* boolean */
	} XtermMouse;
extern	int xt_enabled;		/* state of XTerm-mouse */
extern	XtermMouse xt_mouse;	/* state of XTerm-mouse */
#endif

	int	cmdch(
			int *	cntp
			)
			;

	/* dumptty.c -------------------------------------------------- */
	void	dumptty(
			FILE *	fp,
			char *	s
			)
			;

	/* dumpwin.c -------------------------------------------------- */
	void	dumpwin(
			WINDOW *	w,
			char *	s
			)
			;

	/* erasechar.c ------------------------------------------------ */
#if	!defined(HAVE_ERASECHAR) && !defined(erasechar) && !defined(TESTING_CONFIG_H)
	int	erasechar(void)
			;
#endif
	int	eraseword(void)
			;

	/* is_xterm.c ------------------------------------------------- */
#if	!defined(HAVE_IS_XTERM)
	int	is_xterm(void)
			;
#endif

	/* killchar.c ------------------------------------------------- */
#if	!defined(HAVE_KILLCHAR) && !defined(killchar) && !defined(TESTING_CONFIG_H)
	int	killchar(void)
			;
#endif

	/* lnextchr.c ------------------------------------------------- */
	int	lnext_char(void)
			;

	/* on_winch.c ------------------------------------------------- */
#ifdef SIG_PTYPES
#  if defined(SIGWINCH)
	void	on_winch(
			void (*func)(void))
			;
	void	enable_winch(
			int	enable)
			;
#  else
#    define	on_winch(func)		/* nothing */
#    define	enable_winch(func)	/* nothing */
#  endif
#endif
	/* rawgets.c -------------------------------------------------- */
#ifdef SIG_PTYPES
#  if defined(SIGWINCH)
extern	int	x_rawgets;	/* position, for use when resizing */
extern	int	y_rawgets;
#  endif
#endif
	int	wrawgets (
			WINDOW *	win,
			char *		buffer,
			const char **	prefix,
			int		buffer_len,
			int		field_len,
			int		first_col,
			int		first_mode,
			int		new_line,
			int		fast_q,
			char **		command,
			int		logging
			)
			;

	char *	rawgets_log(void)
			;

#define		 rawgets(b, p, bl, fl, fc, fm, nl, q, cmd, log) \
	wrawgets(stdscr, b, p, bl, fl, fc, fm, nl, q, cmd, log)

	/* rawterm.c -------------------------------------------------- */
	void	save_terminal(void)
			;

	void	restore_terminal(void)
			;

	void	rawterm(void)
			;

	void	cookterm(void)
			;

	/* resizewin.c ------------------------------------------------ */
	int	resizewin(void)
			;

	/* savewin.c -------------------------------------------------- */
	void	savewin(void)
			;

	void	lastwin(
			int	redo,
			int	top
			)
			;

	void	unsavewin(
			int	redo,
			int	top
			)
			;

	/* valid_ch.c ------------------------------------------------- */
	int	valid_curses_char(
	    		int	ch
	    		)
	    		;
	int	valid_shell_char(
	    		int	ch
	    		)
	    		;
	/* win2file.c ------------------------------------------------- */
	void	win2fp(
			WINDOW *	win,
			FILE *	fp,
			const char *	prefix
			)
			;

	int	win2file(
			WINDOW *	win,
			char *	file
			)
			;

	/* wrepaint.c ------------------------------------------------- */
	void	wrepaint(
			WINDOW *	win,
			int	row
			)
			;

	/* wresize.c -------------------------------------------------- */
#if !defined(HAVE_WRESIZE)
	int	wresize(
			WINDOW *	win,
			int	rows,
			int	cols
			)
			;
#endif

#undef extern

#endif	/* TD_CURSE_H */
