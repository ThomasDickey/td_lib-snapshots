#ifndef	lint
static	char	sccs_id[] = "@(#)doalloc.c	1.6 88/05/17 09:57:18";
#endif	lint

/*
 * Author:	T.E.Dickey
 * Created:	09 Jan 1986
 * Modified:
 *		10 May 1988, added 'dofree()' entrypoint for debugging aid.
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

#ifdef	DEBUG
#include	<stdio.h>
static	FILE	*log;
static	char	*allocated[DEBUG];
static	int	used;
#endif	DEBUG

char	*doalloc (oldp, amount)
register char	*oldp;
register unsigned amount;
{
register char	*newp = (oldp != 0) ? realloc(oldp, amount) : malloc(amount);

	if (!newp) failed("doalloc");
#ifdef	DEBUG
	{
	register int	j;
		for (j = 0; j < used; j++)
			if (allocated[j] == 0)
				break;
		allocated[j] = newp;
		if (j >= used)
			used = ++j;
		if (!log) log = fopen("allocate.log", "a+");
		fprintf(log, "  allocate(%#8x,%4d) = %#8x:%d\n",
			oldp, amount, newp, j);
		fflush(log);
	}
#endif	DEBUG
	return (newp);
}

/*
 * Entrypoint so we can validate pointers
 */
dofree(oldp)
char	*oldp;
{
	if (oldp) {
#ifdef	DEBUG
	register int j;
		fprintf(log, "deallocate(%#8x)\n", oldp);
		fflush(log);
		for (j = 0; j < used; j++) {
			if (allocated[j] == oldp) {
				free(oldp);
				allocated[j] = 0;
				return;
			}
		}
#else	DEBUG
		free(oldp);
		return;
#endif	DEBUG
	}
	printf("free(%#x) not found\r\n", oldp);
#if	defined(apollo) && defined(DEBUG)
	/* force a walkback */
	{
	char	msg[80];
		(void)sprintf(msg, "/com/tb %d", getpid());
		(void)system(msg);
	}
#endif	apollo
}
