/* $Id: td_curse.h,v 12.27 1995/03/31 02:00:05 tom Exp $ */

/*
 * TD_LIB CURSES-related definitions
 */

#ifndef		TD_CURSE_H
#define		TD_CURSE_H

#ifndef		PTYPES_H
#include "ptypes.h"
#endif		/* PTYPES_H */

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
#endif

#include	<curses.h>

#if HAVE_TERMCAP_H
#include <termcap.h>
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
 *
 * The SYS5_CURSES definition is used only for places where we must assume,
 * rather than provide an explicit test.  Some implementations will break this
 * assumption.
 *
 * On MS-DOS, I've used PDCurses 2.x, which is derived from NCurses.
 */
#if defined(MSDOS) || HAVE_KEYPAD
#define SYS5_CURSES 1
#else
#define SYS5_CURSES 0
#endif

/*----------------------------------------------------------------------------*/
/*
 * The SYS5 clone "ncurses" 1.8.5 has an off-by-one error in the computation of
 * the _maxy/_maxx values.  These macros are used to hide this bug.
 */
#define	wMaxX(w)	(((w)->_maxx) + (COLS  - stdscr->_maxx))
#define	wMaxY(w)	(((w)->_maxy) + (LINES - stdscr->_maxy))

/*----------------------------------------------------------------------------*/
#if CURSES_LIKE_BSD
#define CursesLine(win,y)	(win)->_y[y]
#define CursesFirstCh(win,y)	(win)->_firstch
#define CursesLastCh(win,y)	(win)->_lastch
#endif
#if CURSES_LIKE_SYSV
#define CursesLine(win,y)	(win)->_line[y]
#define CursesFirstCh(win,y)	(win)->_firstchar
#define CursesLastCh(win,y)	(win)->_lastchar
#endif
#if CURSES_LIKE_NCURSES		/* ncurses 1.8.8 and later */
#define CursesLine(win,y)	(win)->_line[y].text
#define CursesFirstCh(win,y)	(win)->_line[y].firstchar
#define CursesLastCh(win,y)	(win)->_line[y].lastchar
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

#define	ARO_UP		ARO('U')
#define	ARO_DOWN	ARO('D')
#define	ARO_LEFT	ARO('L')
#define	ARO_RIGHT	ARO('R')

#define ARO_MOUSE       ARO('M')

/*
 * note: System5 curses does not define the 'screen' structure
 */
#if !HAVE_TYPE_CHTYPE
	typedef	char	chtype;		/* sys5-curses data-type */
#endif

#if	defined(lint) && NEED_STRUCT_SCREEN
	struct	screen	{ int dummy; };
#endif

#ifndef beep
#if HAVE_BEEP && NEED_BEEP
extern	int	beep		ARGS((void));
#endif /* HAVE_BEEP */
#endif

#if HAVE_ENDWIN && NEED_ENDWIN
extern	int	endwin		ARGS((void));
#endif /* HAVE_ENDWIN */
#if HAVE_PRINTW && NEED_PRINTW
extern	int	printw		ARGS((char *f, ...));
#endif /* HAVE_PRINTW */
#if USING_SGTTY_H
#if HAVE_STTY && NEED_STTY
extern	int	stty		ARGS((int f, struct sgttyb *p));
#endif /* HAVE_STTY */
#endif
#if HAVE_TGETENT && NEED_TGETENT
extern	int	tgetent		ARGS((char *p, const char *t));
#endif /* HAVE_TGETENT */
#if HAVE_TGETNUM && NEED_TGETNUM
extern	int	tgetnum		ARGS((const char *n));
#endif /* HAVE_TGETNUM */
#if HAVE_TGETSTR && NEED_TGETSTR
extern	char *	tgetstr		ARGS((const char *n, char **p));
#endif /* HAVE_TGETSTR */
#if HAVE_TOUCHWIN && NEED_TOUCHWIN
extern	int	touchwin	ARGS((WINDOW *w));
#endif /* HAVE_TOUCHWIN */
#if HAVE_WADDCH && NEED_WADDCH
extern	int	waddch		ARGS((WINDOW *w, chtype c));
#endif /* HAVE_WADDCH */
#if HAVE_WADDSTR && NEED_WADDSTR
extern	int	waddstr		ARGS((WINDOW *w, char *s));
#endif /* HAVE_WADDSTR */
#if HAVE_WCLRTOBOT && NEED_WCLRTOBOT
extern	int	wclrtobot	ARGS((WINDOW *w));
#endif /* HAVE_WCLRTOBOT */
#if HAVE_WCLRTOEOL && NEED_WCLRTOEOL
extern	int	wclrtoeol	ARGS((WINDOW *w));
#endif /* HAVE_WCLRTOEOL */
#if HAVE_WGETCH && NEED_WGETCH
extern	int	wgetch		ARGS((WINDOW *w));
#endif /* HAVE_WGETCH */
#if HAVE_WMOVE && NEED_WMOVE
extern	int	wmove		ARGS((WINDOW *w, int ybase, int xbase));
#endif /* HAVE_WMOVE */
#if HAVE_WREFRESH && NEED_WREFRESH
extern	int	wrefresh	ARGS((WINDOW *w));
#endif /* HAVE_WREFRESH */
#if HAVE_WSTANDEND && NEED_WSTANDEND
extern	int	wstandend	ARGS((WINDOW *w));
#endif /* HAVE_WSTANDEND */
#if HAVE_WSTANDOUT && NEED_WSTANDOUT
extern	int	wstandout	ARGS((WINDOW *w));
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

#if LINTLIBRARY
#define extern	/* nothing */
#endif
	/* addchnst.c ------------------------------------------------- */
#if	!HAVE_ADDCHNSTR && !defined(TESTING_CONFIG_H)
	int	waddchnstr(
			_arx(WINDOW *,	w)
			_arx(chtype *,	s)
			_ar1(int,	n)
				)
			_dcl(WINDOW *,	w)
			_dcl(chtype *,	s)
			_dcl(int,	n)
			_ret
#define addchnstr(str,n)	waddchnstr(stdscr,str, n)
#endif

	/* beep.c ----------------------------------------------------- */
#if	!HAVE_BEEP && !defined(TESTING_CONFIG_H)
	void	beep(_ar0)
			_nul
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
extern	XtermMouse xt_mouse;	/* state of XTerm-mouse */
#endif

	int	cmdch(
			_ar1(int *,	cntp)
			)
			_dcl(int *,	cntp)
			_ret

	/* dumptty.c -------------------------------------------------- */
	void	dumptty(
			_arx(FILE *,	fp)
			_ar1(char *,	s)
			)
			_dcl(FILE *,	fp)
			_dcl(char *,	s)
			_nul

	/* dumpwin.c -------------------------------------------------- */
	void	dumpwin(
			_arx(WINDOW *,	w)
			_ar1(char *,	s)
			)
			_dcl(WINDOW *,	w)
			_dcl(char *,	s)
			_nul

	/* erasechar.c ------------------------------------------------ */
#if	!HAVE_ERASECHAR && !defined(erasechar) && !defined(TESTING_CONFIG_H)
	int	erasechar(_ar0)
			_ret
#endif
	int	eraseword(_ar0)
			_ret

	/* killchar.c ------------------------------------------------- */
#if	!HAVE_KILLCHAR && !defined(killchar) && !defined(TESTING_CONFIG_H)
	int	killchar(_ar0)
			_ret
#endif

	/* on_winch.c ------------------------------------------------- */
#ifdef SIG_PTYPES
#  if SIGWINCH
	void	on_winch(
			_fn1(void,	func,(void)))
			_dcl(void,	(*func)())
			_nul
#  else
#    define	on_winch(func)	/* nothing */
#  endif
#endif
	/* rawgets.c -------------------------------------------------- */
#ifdef SIG_PTYPES
#  if SIGWINCH
extern	int	x_rawgets;	/* position, for use when resizing */
extern	int	y_rawgets;
#  endif
#endif
	int	wrawgets (
			_arx(WINDOW *,	win)
			_arx(char *,	buffer)
			_arx(char **,	prefix)
			_arx(int,	buffer_len)
			_arx(int,	field_len)
			_arx(int,	first_col)
			_arx(int,	first_mode)
			_arx(int,	newline)
			_arx(int,	fast_q)
			_arx(char **,	command)
			_ar1(int,	logging)
			)
			_dcl(WINDOW *,	win)
			_dcl(char *,	buffer)
			_dcl(char **,	prefix)
			_dcl(int,	buffer_len)
			_dcl(int,	field_len)
			_dcl(int,	first_col)
			_dcl(int,	first_mode)
			_dcl(int,	newline)
			_dcl(int,	fast_q)
			_dcl(char **,	command)
			_dcl(int,	logging)
			_ret

	char *	rawgets_log(_ar0)
			_ret

#define		 rawgets(b, p, bl, fl, fc, fm, nl, q, cmd, log) \
	wrawgets(stdscr, b, p, bl, fl, fc, fm, nl, q, cmd, log)

	/* rawterm.c -------------------------------------------------- */
	void	save_terminal(_ar0)
			_nul

	void	restore_terminal(_ar0)
			_nul

	void	rawterm(_ar0)
			_nul

	void	cookterm(_ar0)
			_nul

	/* resizewin.c ------------------------------------------------ */
	int	resizewin(_ar0)
			_ret

	/* savewin.c -------------------------------------------------- */
	void	savewin(_ar0)
			_nul

	void	lastwin(
			_arx(int,	redo)
			_ar1(int,	top)
			)
			_dcl(int,	redo)
			_dcl(int,	top)
			_nul

	void	unsavewin(
			_arx(int,	redo)
			_ar1(int,	top)
			)
			_dcl(int,	redo)
			_dcl(int,	top)
			_nul

	/* win2file.c ------------------------------------------------- */
	void	win2fp(
			_arx(WINDOW *,	win)
			_arx(FILE *,	fp)
			_ar1(char *,	prefix)
			)
			_dcl(WINDOW *,	win)
			_dcl(FILE *,	fp)
			_dcl(char *,	prefix)
			_nul

	int	win2file(
			_arx(WINDOW *,	win)
			_ar1(char *,	file)
			)
			_dcl(WINDOW *,	win)
			_dcl(char *,	file)
			_ret

	/* wrepaint.c ------------------------------------------------- */
	void	wrepaint(
			_arx(WINDOW *,	win)
			_ar1(int,	row)
			)
			_dcl(WINDOW *,	win)
			_dcl(int,	row)
			_nul

	/* wresize.c -------------------------------------------------- */
	void	wresize(
			_arx(WINDOW *,	win)
			_arx(int,	rows)
			_ar1(int,	cols)
			)
			_dcl(WINDOW *,	win)
			_dcl(int,	rows)
			_dcl(int,	cols)
			_nul

#undef extern

#endif	/* TD_CURSE_H */
