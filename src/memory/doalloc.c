#ifndef	lint
static	char	Id[] = "$Id: doalloc.c,v 9.0 1991/05/15 10:16:03 ste_cm Rel $";
#endif

/*
 * Author:	T.E.Dickey
 * Created:	09 Jan 1986
 * $Log: doalloc.c,v $
 * Revision 9.0  1991/05/15 10:16:03  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  10:16:03  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  89/06/09  14:27:50  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  89/06/09  14:27:50  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/06/09  14:27:50  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/06/09  14:27:50  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  89/06/09  14:27:50  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  89/06/09  14:27:50  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.2  89/06/09  14:27:50  dickey
 *		provided walkback-case for vms; made trace & testing separate ifdef's
 *		
 *		Revision 2.1  89/04/24  16:39:59  dickey
 *		ifdef'd for port to vms
 *		
 *		Revision 1.9  88/08/10  12:19:33  dickey
 *		sccs2rcs keywords
 *		
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
logit(msg, num)
{
	static	FILE	*log;

	if (!log)
		log = fopen("doalloc.log", "a+");
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
dofree(oldp)
char	*oldp;
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
main()
{
	dofree(-1);		/* make sure walkback works */
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	/* TEST */
