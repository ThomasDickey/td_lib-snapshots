/* $Id: td_curse.h,v 12.3 1993/11/01 20:07:59 dickey Exp $ */

/*
 * TD_LIB CURSES-related definitions
 */

#ifndef		_CM_CURSES_H
#define		_CM_CURSES_H

#ifndef		_PTYPES_
#include "ptypes.h"
#endif		/* _PTYPES_ */

#include	<curses.h>

/*
 * note: System5 curses does not define the 'screen' structure
 */
#ifdef	SYSTEM5
# ifdef	lint
	struct	screen	{ int dummy; };
# endif
#else
	typedef	char	chtype;		/* sys5-curses data-type */
# ifndef erasechar
	extern	int	erasechar(_ar0); */
# endif
# ifndef killchar
	extern	int	killchar(_ar0); */
# endif
#endif	/* SYSTEM5 */

#if !defined(__hpux) && !defined(LINTLIBRARY)	/* defined in <curses.h> */
extern	char	*tgetstr(_arx(char *,n) _ar1(char **,p));
#endif

#if defined(__GNUC__) && defined(sun)
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
#if	!defined(SYSTEM5)
	void	beep(_ar0)
			_nul
#endif

	/* blip.c ----------------------------------------------------- */
	void	blip(
			_ar1(int,	c)
			)
			_dcl(int,	c)
			_nul

	/* cmdch.c ---------------------------------------------------- */
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
#if	!defined(SYSTEM5)
#ifdef	erasechar
#undef	erasechar
#endif	/* erasechar */
	int	erasechar(_ar0)
			_ret
#endif
	int	eraseword(_ar0)
			_ret

	/* killchar.c ------------------------------------------------- */
#if	!defined(SYSTEM5)
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
	void	rawterm(_ar0)
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

#endif		/* _CM_CURSES_H */
