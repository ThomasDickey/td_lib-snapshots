#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/process/RCS/catchall.c,v 2.0 1988/09/13 07:32:12 ste_cm Exp $";
#endif	lint

/*
 * Title:	catchall.c (catch all signals)
 * Author:	T.E.Dickey
 * Created:	13 Sep 1988
 * $Log: catchall.c,v $
 * Revision 2.0  1988/09/13 07:32:12  ste_cm
 * BASELINE Thu Apr  6 09:45:13 EDT 1989
 *
 *		Revision 1.2  88/09/13  07:32:12  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	catches all (catchable, anyway) signals which would otherwise
 *		terminate the current process.  We expect that the signal-
 *		catching function is a cleanup handler (i.e., removes temporary
 *		files).
 *
 *		If the given function-pointer is null, we revert to the
 *		previously-saved set of signal-arguments.  Note that SIG_DFL
 *		is the same as 0.
 */

#include	<sys/signal.h>

catchall(catchsig)
int	(*catchsig)();
{
#define	SAVE(j)	save[j].func
	static	struct	{ int	(*func)(); } save[NSIG+1];
	static	char	sigs[] = {
				SIGINT,
				SIGHUP,
				SIGQUIT,
				SIGPIPE,
				SIGTERM
			};
	register int j;

	for (j = 0; j < sizeof(sigs); j++) {
		if (catchsig == 0) {
			(void)signal(sigs[j], SAVE(j));
		} else {
			SAVE(j) = signal(sigs[j], catchsig);
		}
	}
}
