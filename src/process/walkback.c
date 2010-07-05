/*
 * Author:	T.E.Dickey
 * Created:	18 Nov 1992, from 'doalloc.c'
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		20 Nov 1992, added test-driver.
 *
 * Function:	Generate a walkback (if possible!)
 */

#define	SIG_PTYPES
#define	STR_PTYPES
#include	"ptypes.h"
#include	<errno.h>

MODULE_ID("$Id: walkback.c,v 12.10 2010/07/05 16:17:46 tom Exp $")

#ifdef	SYS_UNIX

#ifdef	apollo
static
int
contains(
	    char *ref,
	    char *tst)
{
    size_t len = strlen(ref);
    while (*tst) {
	if (!strncmp(ref, tst++, len))
	    return TRUE;
    }
    return FALSE;
}
#else /* assume we can dump-core */
static const char *core = "core";
static char *caller;
#endif

/******************************************************************************/
	/*ARGSUSED */
void
walkback(
	    char *program)
{
    static int count;
    if (program == 0)
	PRINTF("WALKBACK %d\n", ++count);

    if (program) {
#if	defined(SYS_UNIX) && !defined(apollo)
	char dot[MAXPATHLEN], bfr[256];
	if (getcwd(dot, sizeof(dot) - 1) == 0)
	    strcpy(dot, ".");
	(void) which(bfr, sizeof(bfr), program, dot);
	caller = stralloc(bfr);
#endif
    } else {			/* do the actual walkback */
#ifdef	vms
	*((char *) 0) = 0;	/* patch */
#else /* SYS_UNIX ? */
#ifdef	apollo
	static char *first = "\"walkback\"", *last = "\"unix_$main\"";
	FILE *pp;
	char bfr[BUFSIZ];
	int ok = FALSE;

	FORMAT(bfr, "/com/tb %d", getpid());
	if (!(pp = popen(bfr, "r")))
	    failed(bfr);

	while (fgets(bfr, sizeof(bfr), pp)) {
	    if (ok && contains(last, bfr))
		break;
	    else if (contains(first, bfr))
		ok = TRUE;
	    else if (ok)
		PRINTF("%s", bfr);
	}
	FCLOSE(pp);
#else /* !apollo */
	FILE *pp;
	char bfr[BUFSIZ];
	int pid;

	FFLUSH(stdout);
	FFLUSH(stderr);
	(void) unlink(core);

	if ((pid = fork()) > 0) {
	    (void) kill(pid, SIGABRT);
	} else {
	    catchall(SIG_DFL);
	    for (;;) ;
	    /*NOTREACHED */
	}

	FORMAT(bfr, "adb %s", caller);
	if ((pp = popen(bfr, "w")) != NULL) {
	    (void) fputs("$c\n", pp);
	    FFLUSH(pp);
	    (void) fputs("$q\n", pp);
	    FFLUSH(pp);
	    (void) pclose(pp);
	    (void) unlink(core);
	}
#endif /* apollo */
#endif /* vms/SYS_UNIX */
    }
}

#ifdef	TEST
static void
do_test(void)
{
    walkback((char *) 0);
}

_MAIN
{
    (void) argc;

    walkback(argv[0]);
    do_test();
    walkback((char *) 0);
    exit(SUCCESS);
}
#endif /* TEST */

#endif /* SYS_UNIX */
