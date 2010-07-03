/*
 * Author:	T.E.Dickey
 * Created:	09 Jan 1986
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
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

MODULE_ID("$Id: doalloc.c,v 12.9 2010/07/03 15:53:42 tom Exp $")

static long count_alloc, count_freed;

static void
fail_alloc(const char *msg, void *ptr)
{
    PRINTF("%s: %p\n", msg, ptr);
#ifdef	SYS_UNIX
    walkback((char *) 0);
#endif
    FFLUSH(stdout);
    FFLUSH(stderr);
    abort();
}

#ifdef	TEST
#ifndef	DEBUG
#define	DEBUG	100
#endif
#endif

#ifdef	DEBUG
typedef struct {
    long size;			/* ...its size */
    void *text;			/* the actual segment */
    int note;			/* ...last value of 'count_alloc' */
} AREA;

static AREA area[DEBUG];

static long maxAllocated,	/* maximum # of bytes allocated */
  nowAllocated,			/* current # of bytes allocated */
  nowPending,			/* current end of 'area[]' table */
  maxPending;			/* maximum # of segments allocated */

static int
FindArea(void *ptr)
{
    register int j;
    for (j = 0; j < DEBUG; j++)
	if (area[j].text == ptr) {
	    if (j >= nowPending) {
		nowPending = j + 1;
		if (nowPending > maxPending)
		    maxPending = nowPending;
	    }
	    return j;
	}
    return -1;
}

static
int
record_freed(void *ptr)
{
    register int j;
    if ((j = FindArea(ptr)) >= 0) {
	nowAllocated -= area[j].size;
	area[j].size = 0;
	area[j].text = 0;
	area[j].note = count_freed;
	if ((j + 1) == nowPending) {
	    register int k;
	    for (k = j; (k >= 0) && !area[k].size; k--)
		nowPending = k;
	}
    }
    return j;
}

static int
record_alloc(void *newp, void *oldp, unsigned len)
{
    register int j;

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
	if ((j = FindArea((void *) 0)) >= 0) {
	    area[j].text = newp;
	    area[j].size = len;
	    area[j].note = count_alloc;
	} else
	    fail_alloc("no room in table", newp);
    }

    nowAllocated += len;
    if (nowAllocated > maxAllocated)
	maxAllocated = nowAllocated;
    return 1;
}

#define	OK_ALLOC(p,q,n)	((p != 0) && (record_alloc(p,q,n) >= 0))
#define	OK_FREE(p)	((p != 0) && (record_freed(p) >= 0))
#else
#define	OK_ALLOC(p,q,n)	(p != 0)
#define	OK_FREE(p)	(p != 0)
#endif /* DEBUG */

#ifdef	DEBUG_LOG
static void
logit(char *msg, int num)
{
    static FILE *log;

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
#endif /* DEBUG_LOG */

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/
void *
doalloc(void *oldp, unsigned amount)
{
    register void *newp;

    count_alloc += (oldp == 0);
    LOGIT("allocate", amount)
	LOGIT("  old = ", oldp)

	newp = (oldp != 0) ? realloc(oldp, amount) : malloc(amount);
    if (!OK_ALLOC(newp, oldp, amount))
	failed("doalloc");

    LOGIT("  new = ", newp)
	return (newp);
}

/*
 * Entrypoint so we can validate pointers
 */
void
dofree(void *oldp)
{
    count_freed++;
    LOGIT("dealloc ", oldp)

	if (OK_FREE(oldp)) {
	free(oldp);
	return;
    }

    fail_alloc("free (not found)", oldp);
}

void
show_alloc(void)
{
    static const char *fmt = ".. %-24.24s %10ld\n";

    PRINTF("** allocator metrics:\n");
    PRINTF(fmt, "allocs:", count_alloc);
    PRINTF(fmt, "frees:", count_freed);
#ifdef	DEBUG
    {
	register int j, count = 0;
	register long total = 0;

	for (j = 0; j < nowPending; j++) {
	    if (area[j].text) {
		if (count++ < 10)
		    PRINTF("...%d) %ld bytes in alloc #%d:%p\n",
			   j,
			   area[j].size,
			   area[j].note,
			   area[j].text);
		total += area[j].size;
	    }
	}
	PRINTF(fmt, "total bytes allocated:", total);
	PRINTF(fmt, "current bytes allocated:", nowAllocated);
	PRINTF(fmt, "maximum bytes allocated:", maxAllocated);
	PRINTF(fmt, "segment-table length:", nowPending);
	PRINTF(fmt, "current # of segments:", count);
	PRINTF(fmt, "maximum # of segments:", maxPending);
    }
#endif
}

#ifdef	TEST
_MAIN
{
    void *p = 0, *q = 0;

    p = doalloc(p, 100);
    dofree(p);

    p = doalloc(q, 200);
    p = doalloc(p, 100);
    p = doalloc(p, 300);
    q = doalloc(q, 300);

    dofree(q);
    dofree(p);

    show_alloc();

    exit(SUCCESS);
    /*NOTREACHED */
}
#endif /* TEST */
