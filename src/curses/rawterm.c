#ifndef	lint
static	char	sccs_id[] = "@(#)rawterm.c	1.3 88/07/28 06:53:42";
#endif	lint

/*
 * Title:	rawterm.c (set terminal to raw mode)
 * Author:	T.E.Dickey
 * Created:	24 Nov 1987
 * Modified:
 *		28 Jul 1988, added 'nl()' to items affected.
 *
 * Function:	Set terminal to raw & noecho at the same time to try to
 *		make apollo kludge timing better.
 */

#include	<curses.h>

/*
 * Set terminal to single-character mode
 */
rawterm()
{
#if	defined(apollo) && defined(noecho)
	_tty.sg_flags |=  RAW;		/* raw()	*/
	_tty.sg_flags &= ~ECHO;		/* noecho()	*/
	_tty.sg_flags &= ~CRMOD;	/* nonl()	*/
	_pfast   =
	_rawmode = TRUE;
	_echoit  = FALSE;
	(void)stty(_tty_ch, &_tty);	/* single op avoids timing bug */
#else
	raw();			/* procedures, not macros (e.g., sys5) */
	noecho();
	nonl();
#endif
}
