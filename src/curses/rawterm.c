#ifndef	NO_SCCS_ID
static	char	sccs_id[] = "@(#)rawterm.c	1.2 88/05/17 13:13:05";
#endif	NO_SCCS_ID

/*
 * Title:	rawterm.c (set terminal to raw mode)
 * Author:	T.E.Dickey
 * Created:	24 Nov 1987
 * Modified:
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
#ifdef	apollo
#ifdef	noecho
	_tty.sg_flags |=  RAW;
	_tty.sg_flags &= ~ECHO;
	_pfast   =
	_rawmode = TRUE;
	_echoit  = FALSE;
	(void)stty(_tty_ch, &_tty);	/* single op avoids timing bug */
#else	noecho
	raw();			/* procedures, not macros (e.g., sys5) */
	noecho();
#endif	noecho
#else	apollo
	raw();
	noecho();
#endif	apollo
}
