#ifndef	lint
static	char	sccs_id[] = "@(#)doalloc.c	1.4 87/05/07 07:58:28";
#endif	lint

/*
 * Author:	T.E.Dickey
 * Created:	09 Jan 1986
 * Modified:
 *		07 May 1987, to use generic 'failed()' as common exit.
 *
 * Function:	Combine malloc/realloc in a consistent manner, checking to
 *		ensure that we got the amount of memory which we requested.
 *
 * Arguments:	oldp	- Old pointer (must be initialized by caller to 0).
 *		amount	- Number of bytes/chars to allocate.
 *
 * Returns:	New pointer, unless procedure fails (then we simply exit).
 */

extern	char	*malloc (), *realloc ();

char	*doalloc (oldp, amount)
register char	*oldp;
register unsigned amount;
{
register char	*newp = (oldp ? realloc(oldp, amount) : malloc(amount));

	if (!newp) failed("doalloc");
	return (newp);
}
