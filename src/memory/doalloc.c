#ifndef	lint
static	char	Id[] = "$Id: doalloc.c,v 11.2 1992/11/17 15:58:16 dickey Exp $";
#endif

/*
 * Author:	T.E.Dickey
 * Created:	09 Jan 1986
 * Modified:
 *		16 Nov 1992, added 'show_alloc()'; revised debug-code.
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

static	long	count_alloc,
		count_freed;

static
void	walkback(_AR0)
{
#ifdef	apollo
	char	msg[80];
	FORMAT(msg, "/com/tb %d", getpid());
	FFLUSH(stdout);
	FFLUSH(stderr);
	(void)system(msg);
#endif	/* apollo */
#ifdef	vms
	*((char *)0) = 0;	/* patch */
#endif	/* vms */
}

static
void	fail_alloc(
	_ARX(char *,	msg)
	_AR1(char *,	ptr)
		)
	_DCL(char *,	msg)
	_DCL(char *,	ptr)
{
	PRINTF("%s: %#x\n", msg, ptr);
	walkback();
	FFLUSH(stdout);
	FFLUSH(stderr);
	abort();
}

#ifdef	DEBUG
typedef	struct	{
	long	size;	/* ...its size */
	char	*text;	/* the actual segment */
	int	note;	/* ...last value of 'count_alloc' */
	} AREA;
static	AREA	area[DEBUG];
static	long	maxAllocated,	/* maximum # of bytes allocated */
		nowAllocated;	/* current # of bytes allocated */
static	int	now_pending;	/* current end of 'area[]' table */

static
int	FindArea(
	_AR1(char *,	ptr))
	_DCL(char *,	ptr)
{
	register int j;
	for (j = 0; j < DEBUG; j++)
		if (area[j].text == ptr) {
			if ((ptr != 0) && (j >= now_pending))
				now_pending = j+1;
			return j;
		}
	return -1;
}

static
int	record_freed(
	_AR1(char *,	ptr))
	_DCL(char *,	ptr)
{
	register int j;
	if ((j = FindArea(ptr)) >= 0) {
		nowAllocated -= area[j].size;
		area[j].size = 0;
		area[j].text = 0;
		area[j].note = count_freed;
		if (j+1 == now_pending) {
			register int	k;
			for (k = j; k >= 0; k--)
				if (!area[k].size)
					now_pending = k;
		}
	}
	return j;
}

static
int	record_alloc(
	_ARX(char *,	newp)
	_ARX(char *,	oldp)
	_AR1(unsigned,	len)
		)
	_DCL(char *,	newp)
	_DCL(char *,	oldp)
	_DCL(unsigned,	len)
{
	register int	j;

	if (newp == oldp) {
		if ((j = FindArea(oldp)) >= 0) {
			nowAllocated -= area[j].size;
			area[j].size = len;
			area[j].note = count_alloc;
		} else
			fail_alloc("could not find", oldp);
	} else {
		if (oldp != 0)
			record_freed(oldp);
		if ((j = FindArea((char *)0)) >= 0) {
			area[j].text = newp;
			area[j].size = len;
			area[j].note = count_alloc;
		} else
			fail_alloc("no room in table", newp);
	}

	nowAllocated += len;
	if (nowAllocated > maxAllocated)
		maxAllocated = nowAllocated;
}

#define	OK_ALLOC(p,q,n)	((p != 0) && (record_alloc(p,q,n) >= 0))
#define	OK_FREE(p)	((p != 0) && (record_freed(p) >= 0))
#else
#define	OK_ALLOC(p,q,n)	(p != 0)
#define	OK_FREE(p)	(p != 0)
#endif	/* DEBUG */

#ifdef	DEBUG_LOG
static
void	logit(
	_ARX(char *,	msg)
	_AR1(int,	num)
		)
	_DCL(char *,	msg)
	_DCL(int,	num)
{
	static	FILE	*log;

	if (!log)
		log = fopen("doalloc.log", "w");
	FPRINTF(log, "%s %#x\n", msg, num);
	FFLUSH(log);
}
#define	LOGIT(msg,num)	logit(msg, (long)num);
#else
#ifdef	DEBUG2
#define	LOGIT(msg,num)	PRINTF("%s %#x\n", msg, num);
#else
#define	LOGIT(msg,num)
#endif
#endif	/* DEBUG_LOG */

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/
char *	doalloc (
	_ARX(char *,	oldp)
	_AR1(unsigned,	amount)
		)
	_DCL(register char *,	oldp)
	_DCL(register unsigned,	amount)
{
	register char	*newp;

	count_alloc += (oldp == 0);
	LOGIT("allocate", amount)
	LOGIT("  old = ", oldp)

	newp = (oldp != 0) ? realloc(oldp, amount) : malloc(amount);
	if (!OK_ALLOC(newp,oldp,amount))
		failed("doalloc");

	LOGIT("  new = ", newp)
	return (newp);
}

/*
 * Entrypoint so we can validate pointers
 */
void	dofree(
	_AR1(char *,	oldp))
	_DCL(char *,	oldp)
{
	count_freed++;
	LOGIT("dealloc ", oldp)

	if (OK_FREE(oldp)) {
		free(oldp);
		return;
	}

	fail_alloc("free (not found)", oldp);
}

void	show_alloc(_AR0)
{
	static	char	*fmt = ".. %-24.24s %10ld\n";

	PRINTF("** allocator metrics:\n");
	PRINTF(fmt, "allocs:", count_alloc);
	PRINTF(fmt, "frees:",  count_freed);
#ifdef	DEBUG
	{
		register int	j, count = 0;
		register long	total	= 0;

		for (j = 0; j < now_pending; j++)
			if (area[j].text) {
				if (count++ < 10)
					PRINTF("...%d) %d bytes in alloc #%d:%#x\n",
						j,
						area[j].size,
						area[j].note,
						area[j].text);
				total += area[j].size;
			}
		PRINTF("...%d:%d segments allocated\n", count, now_pending);
		PRINTF(fmt, "total bytes allocated:",   total);
		PRINTF(fmt, "current bytes allocated:", nowAllocated);
		PRINTF(fmt, "maximum bytes allocated:", maxAllocated);
	}
#endif
}

#ifdef	TEST
_MAIN
{
	dofree((char *)-1);		/* make sure walkback works */
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	/* TEST */
