#ifndef	lint
static	char	sccs_id[] = "@(#)doalloc.c	1.1 86/01/09 08:21:07";
#endif	lint

/*
 * Author:	T.E.Dickey
 * Created:	09 Jan 1986
 * Modified:	09 Jan 1986
 *
 * Function:	Combine malloc/realloc in a consistent manner, checking to
 *		ensure that we got the amount of memory which we requested.
 *
 * Arguments:	oldp	- Old pointer (must be initialized by caller to 0).
 *		amount	- Number of bytes/chars to allocate.
 *
 * Returns:	New pointer, unless procedure fails (then we simply exit).
 */

#include	<stdio.h>

char	*malloc (),
	*realloc ();

char	*doalloc (oldp, amount)
register char	*oldp;
register int	amount;
{
register char	*newp = (oldp ? realloc(oldp, amount) : malloc(amount));

	if (!newp) {
		beep ();
		fprintf (stderr, "Could not (re)allocate %d bytes\n", amount);
		fflush (stderr);
		sleep (3);
		exit (1);
	}
	return (newp);
}
