#ifndef	lint
static	char	Id[] = "$Id: walkback.c,v 12.2 1993/09/27 17:06:55 dickey Exp $";
#endif

/*
 * Author:	T.E.Dickey
 * Created:	18 Nov 1992, from 'doalloc.c'
 * Modified:
 *		21 Sep 1993, gcc-warnings
 *		20 Nov 1992, added test-driver.
 *
 * Function:	Generate a walkback (if possible!)
 */

#define	SIG_PTYPES
#define	STR_PTYPES
#include	"ptypes.h"
#include	<errno.h>

/******************************************************************************/
#ifdef	unix
#ifdef	apollo
static
int	contains(
	_ARX(char *,	ref)
	_AR1(char *,	tst)
		)
	_DCL(char *,	ref)
	_DCL(char *,	tst)
{
	size_t	len	= strlen(ref);
	while (*tst) {
		if (!strncmp(ref,tst++,len))
			return TRUE;
	}
	return FALSE;
}
#else	/* assume we can dump-core */
static	char	*core	= "core";
static	char	*caller;
#endif
#endif	/* unix */

/******************************************************************************/
	/*ARGSUSED*/
void	walkback(
	_AR1(char *,	program))
	_DCL(char *,	program)
{
	static	int	count;
	if (program == 0)
		PRINTF("WALKBACK %d\n", ++count);

	if (program) {
#if	defined(unix) && !defined(apollo)
		char	dot[MAXPATHLEN],
			bfr[256];
		(void)getwd(dot);
		(void)which(bfr, sizeof(bfr), program, dot);
		caller = stralloc(bfr);
#endif
	} else {	/* do the actual walkback */
#ifdef	vms
		*((char *)0) = 0;	/* patch */
#else	/* unix ? */
#ifdef	apollo
		static	char	*first	= "\"walkback\"",
				*last	= "\"unix_$main\"";
		FILE	*pp;
		char	bfr[BUFSIZ];
		int	ok	= FALSE;

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
#else	/* !apollo */
		FILE	*pp;
		char	bfr[BUFSIZ];
		int	pid;

		FFLUSH(stdout);
		FFLUSH(stderr);
		(void)unlink(core);

		if ((pid = fork()) > 0) {
			(void)kill(pid, SIGABRT);
		} else {
			catchall(SIG_DFL);
			for(;;);
			/*NOTREACHED*/
		}

		FORMAT(bfr, "adb %s", caller);
		if ((pp = popen(bfr, "w")) != NULL) {
			(void)fputs("$c\n", pp); FFLUSH(pp);
			(void)fputs("$q\n", pp); FFLUSH(pp);
			(void)pclose(pp);
			(void)unlink(core);
		}
#endif	/* apollo */
#endif	/* vms/unix */
	}
}

#ifdef	TEST
static
void	do_test(_AR0)
{
	walkback((char *)0);
}

_MAIN
{
	walkback(argv[0]);
	do_test();
	walkback((char *)0);
	exit(SUCCESS);
}
#endif
