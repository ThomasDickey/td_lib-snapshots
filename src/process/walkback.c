#ifndef	lint
static	char	Id[] = "$Id: walkback.c,v 11.3 1992/11/20 14:06:37 dickey Exp $";
#endif

/*
 * Author:	T.E.Dickey
 * Created:	18 Nov 1992, from 'doalloc.c'
 * Modified:
 *		20 Nov 1992, added test-driver.
 *
 * Function:	Generate a walkback (if possible!)
 */

#include	"ptypes.h"

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
#endif

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
#else
		fprintf(stderr, "walkback not implemented\n");
		fflush(stderr);
		abort();
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
