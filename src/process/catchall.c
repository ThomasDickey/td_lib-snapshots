#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: catchall.c,v 12.3 1993/11/26 13:28:40 dickey Exp $";
#endif

/*
 * Title:	catchall.c (catch all signals)
 * Author:	T.E.Dickey
 * Created:	13 Sep 1988
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		04 Oct 1991, conversion to ANSI
 *		15 May 1991, lint (SunOs), mods to compile under apollo sr10.3
 *		31 Oct 1989, recompiled on sun3 (release 4.0.x)
 *		10 May 1989, compiled on sun/sparc
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

#define	SIG_PTYPES
#include "ptypes.h"

void	catchall(
	_FN1(SIG_T,	catchsig,	(SIGNAL_ARGS)))
	_DCL(SIG_T,	(*catchsig)())
{
#define	SAVE(j)	save[j].func
	static	struct	{ DCL_SIGNAL(func); } save[NSIG+1];
	static	char	sigs[] = {
				SIGINT
				,SIGTERM
#ifdef unix
				,SIGHUP
				,SIGQUIT
				,SIGPIPE
#endif
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
