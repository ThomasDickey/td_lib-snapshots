#ifndef	lint
static	char	sccs_id[] = "@(#)doalloc.c	1.3 86/10/14 08:50:18";
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

#include	<syscap.h>

#include	<stdio.h>
extern	unsigned sleep();
extern	char	*malloc (), *realloc ();

#ifdef	SYS3_LLIB
extern	int	exit();
#else
extern	void	exit();
#endif

char	*doalloc (oldp, amount)
register char	*oldp;
register unsigned amount;
{
register char	*newp = (oldp ? realloc(oldp, amount) : malloc(amount));

	if (!newp) {
		beep ();
		(void) fprintf (stderr, "Could not (re)allocate %d bytes\n", amount);
		(void) fflush (stderr);
		(void) sleep (3);
		resetterm();
		(void) exit (1);
	}
	return (newp);
}
