/* $Id: td_curse.h,v 12.9 1994/05/21 18:41:39 tom Exp $ */

/*
 * TD_LIB CURSES-related definitions
 */

#ifndef		_TD_CURSE_H
#define		_TD_CURSE_H

#ifndef		_PTYPES_
#include "ptypes.h"
#endif		/* _PTYPES_ */

#ifdef USE_NCURSES
#include	<ncurses/ncurses.h>
#else
#include	<curses.h>
#endif

/*----------------------------------------------------------------------------*/
#if defined(SYSTEM5) || defined(MSDOS) || defined(__svr4__) || defined(USE_NCURSES)
#define SYS5_CURSES 1
#else
#define SYS5_CURSES 0
#endif

/*----------------------------------------------------------------------------*/
#if defined(linux)
#define HAS_CURSES_BEEP 1
#endif

#ifndef HAS_CURSES_BEEP
#define HAS_CURSES_BEEP SYS5_CURSES
#endif

/*----------------------------------------------------------------------------*/
#if defined(linux) && !defined(USE_NCURSES)
#define HAS_CURSES_KEYPAD 0
#endif

#ifndef HAS_CURSES_KEYPAD
#define HAS_CURSES_KEYPAD SYS5_CURSES
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
#if SYS5_CURSES
# if defined(linux) && !USE_NCURSES
	typedef	char	chtype;		/* sys5-curses data-type */
# endif
# ifdef	lint
	struct	screen	{ int dummy; };
# endif
#else
# ifndef __svr4__
	typedef	char	chtype;		/* sys5-curses data-type */
# ifndef erasechar
	extern	int	erasechar(_ar0); */
# endif
# ifndef killchar
	extern	int	killchar(_ar0); */
# endif
# endif /* __svr4__ */
#endif	/* SYSTEM5 */

#if !(defined(__hpux) || defined(linux) || defined(LINTLIBRARY)) /* defined in <curses.h> */
extern	char	*tgetstr(_arx(char *,n) _ar1(char **,p));
#endif

#if defined(__GNUC__) && defined(sun) && !defined(__svr4__)
extern	int	stty		(int, struct sgttyb *);
extern	int	endwin		(void);
extern	int	printw		(char *, ...);
extern	int	touchwin	(WINDOW *);
extern	int	waddch		(WINDOW *, int);
extern	int	waddstr		(WINDOW *, char *);
extern	int	wclrtobot	(WINDOW *);
extern	int	wclrtoeol	(WINDOW *);
extern	int	wgetch		(WINDOW *);
extern	int	wmove		(WINDOW *, int ybase, int xbase);
extern	int	wstandend	(WINDOW *);
extern	int	wstandout	(WINDOW *);
extern	int	wrefresh	(WINDOW *);
#endif

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

	/* beep.c ----------------------------------------------------- */
#if	!HAS_CURSES_BEEP
	void	beep(_ar0)
			_nul
#endif

	/* cmdch.c ---------------------------------------------------- */
#ifndef	NO_XTERM_MOUSE
	struct	{
	int	col;		/* 0 .. COLS-1 */
	int	row;		/* 0 .. LINES-1 */
	int	button;		/* 1, 2 or 3 */
	int	pressed;	/* boolean */
	int	released;	/* boolean */
	int	dbl_clik;	/* boolean */
	} xt_mouse;	/* state of XTerm-mouse */
#endif

	int	cmdch(
			_ar1(int *,	cntp)
			)
			_dcl(int *,	cntp)
			_ret

	/* dumpwin.c -------------------------------------------------- */
	void	dumpwin(
			_arx(WINDOW *,	w)
			_ar1(char *,	s)
			)
			_dcl(WINDOW *,	w)
			_dcl(char *,	s)
			_nul

	/* erasechar.c ------------------------------------------------ */
#if	!SYS5_CURSES
#ifdef	erasechar
#undef	erasechar
#endif	/* erasechar */
	int	erasechar(_ar0)
			_ret
#endif
	int	eraseword(_ar0)
			_ret

	/* killchar.c ------------------------------------------------- */
#if	!SYS5_CURSES
#ifdef	killchar
#undef	killchar
#endif	/* killchar */
	int	killchar(_ar0)
			_ret
#endif

	/* rawgets.c -------------------------------------------------- */
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

#endif		/* _TD_CURSE_H */
