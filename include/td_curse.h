/* $Id: td_curse.h,v 12.0 1993/04/26 15:27:21 ste_cm Rel $ */

/*
 * TD_LIB CURSES-related definitions
 */

#ifndef		_CM_CURSES_H
#define		_CM_CURSES_H

#ifndef		_PTYPES_
#include "ptypes.h"
#endif		/* _PTYPES_ */

/*
 * note: System5 curses does not define the 'screen' structure
 */
#ifdef	SYSTEM5
struct	screen	{ int dummy; };
#else	/* SYSTEM5 */
typedef char	chtype;		/* sys5-curses data-type */
#ifndef	erasechar
extern	char	erasechar();
#endif	/* erasechar */
#ifndef	killchar
extern	char	killchar();
#endif	/* killchar */
#endif	/* SYSTEM5 */
#include	<curses.h>

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
		beep(_ar0)
			_nul

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
		dumpwin(
			_arx(WINDOW *,	w)
			_ar1(char *,	s)
			)
			_dcl(WINDOW *,	w)
			_dcl(char *,	s)
			_nul

	/* erasechar.c ------------------------------------------------ */
#ifdef	erasechar
#undef	erasechar
#endif	/* erasechar */
	char	erasechar(_ar0)
			_ret
	char	eraseword(_ar0)
			_ret

	/* killchar.c ------------------------------------------------- */
#ifdef	killchar
#undef	killchar
#endif	/* killchar */
	char	killchar(_ar0)
			_ret

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
		rawterm(_ar0)
			_nul

	/* resizewin.c ------------------------------------------------ */
	int	resizewin(_ar0)
			_ret

	/* savewin.c -------------------------------------------------- */
		savewin(_ar0)
			_nul

		lastwin(
			_arx(int,	redo)
			_ar1(int,	top)
			)
			_dcl(int,	redo)
			_dcl(int,	top)
			_nul

		unsavewin(
			_arx(int,	redo)
			_ar1(int,	top)
			)
			_dcl(int,	redo)
			_dcl(int,	top)
			_nul

	/* win2file.c ------------------------------------------------- */
	win2fp(
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
	wrepaint(
			_arx(WINDOW *,	win)
			_ar1(int,	row)
			)
			_dcl(WINDOW *,	win)
			_dcl(int,	row)
			_nul

#endif		/* _CM_CURSES_H */
