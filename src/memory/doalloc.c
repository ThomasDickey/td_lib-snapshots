#ifndef	lint
static	char	Id[] = "$Id: doalloc.c,v 11.0 1992/04/02 16:18:34 ste_cm Rel $";
#endif

/*
 * Author:	T.E.Dickey
 * Created:	09 Jan 1986
 * Modified:
 *		02 Apr 1992, don't append to log-file. Write error message there
 *			     too.
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		09 Jun 1989, provided walkback-case for vms; made trace &
 *			     testing separate ifdef's
 *		24 Apr 1989, ifdef'd for port to vms
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

#include	"ptypes.h"

#ifdef	DEBUG
static	char	*allocated[DEBUG];
static	int	used;
#endif

#ifdef	DEBUG_LOG
static
logit(
_ARX(char *,	msg)
_AR1(int,	num)
	)
_DCL(char *,	msg)
_DCL(int,	num)
{
	static	FILE	*log;

	if (!log)
		log = fopen("doalloc.log", "w");
	fprintf(log, "%s %#x\n", msg, num);
	fflush(log);
}
#define	LOGIT(msg,num)	logit(msg, (long)num);
#else
#ifdef	DEBUG
#define	LOGIT(msg,num)	printf("%s %#x\n", msg, num);
#else
#define	LOGIT(msg,num)
#endif
#endif	/* DEBUG_LOG */

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/
char	*
doalloc (
_ARX(char *,	oldp)
_AR1(unsigned,	amount)
	)
_DCL(register char *,	oldp)
_DCL(register unsigned,	amount)
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
		if (j >= used && j < DEBUG)
			used = ++j;
	}
#endif	/* DEBUG */
	LOGIT("allocate", amount)
	LOGIT("  old = ", oldp)
	LOGIT("  new = ", newp)
	return (newp);
}

/*
 * Entrypoint so we can validate pointers
 */
dofree(
_AR1(char *,	oldp))
_DCL(char *,	oldp)
{
	LOGIT("dealloc ", oldp)
	if (oldp) {
#ifdef	DEBUG
		register int j;
		for (j = 0; j < used; j++) {
			if (allocated[j] == oldp) {
				free(oldp);
				allocated[j] = 0;
				return;
			}
		}
#else	/* !DEBUG */
		free(oldp);
		return;
#endif	/* DEBUG */
	}
	(void)printf("free(%#x) not found\r\n", oldp);
	LOGIT("free (not found)", oldp);
#ifdef	DEBUG
#ifdef	apollo
	/* force a walkback */
	{
	char	msg[80];
		(void)sprintf(msg, "/com/tb %d", getpid());
		(void)system(msg);
	}
#endif	/* apollo */
#ifdef	vms
	*((char *)0) = 0;
#endif	/* vms */
#endif	/* DEBUG */
}

#ifdef	TEST
_MAIN
{
	dofree((char *)-1);		/* make sure walkback works */
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	/* TEST */
