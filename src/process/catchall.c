#ifndef	lint
static	char	Id[] = "$Id: catchall.c,v 9.0 1991/05/31 17:28:51 ste_cm Rel $";
#endif

/*
 * Title:	catchall.c (catch all signals)
 * Author:	T.E.Dickey
 * Created:	13 Sep 1988
 * $Log: catchall.c,v $
 * Revision 9.0  1991/05/31 17:28:51  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.2  91/05/31  17:28:51  dickey
 *		lint (SunOs)
 *		
 *		Revision 8.1  91/05/15  11:40:22  dickey
 *		mods to compile under apollo sr10.3
 *		
 *		Revision 8.0  89/10/31  09:19:52  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  89/10/31  09:19:52  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/10/31  09:19:52  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.1  89/10/31  09:19:52  dickey
 *		recompiled on sun3 (release 4.0.x)
 *		
 *		Revision 5.0  89/05/10  14:49:36  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  89/05/10  14:49:36  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  89/05/10  14:49:36  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.1  89/05/10  14:49:36  dickey
 *		compiled on sun/sparc
 *		
 *		Revision 2.0  88/09/13  07:32:12  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
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

catchall(catchsig)
DCL_SIGNAL(catchsig);
{
#define	SAVE(j)	save[j].func
	static	struct	{ DCL_SIGNAL(func); } save[NSIG+1];
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
