#ifndef	lint
static	char	sccs_id[] = "@(#)rawterm.c	1.4 88/08/03 06:20:48";
#endif	lint

/*
 * Title:	rawterm.c (set terminal to raw mode)
 * Author:	T.E.Dickey
 * Created:	24 Nov 1987
 * Modified:
 *		03 Aug 1988, use 'crmode()' rather than 'raw()' so we can see
 *			     signals in caller.
 *		28 Jul 1988, added 'nl()' to items affected.
 *
 * Function:	Set terminal to single-character mode
 */

#include	<curses.h>

/*
 * Set terminal to single-character mode
 */
rawterm()
{
	crmode();
	noecho();
	nonl();
}
