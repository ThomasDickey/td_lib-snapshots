#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/process/RCS/for_user.c,v 8.0 1989/03/31 15:22:36 ste_cm Rel $";
#endif	lint

/*
 * Title:	for_user.c (carry out function for set-uid user)
 * Author:	T.E.Dickey
 * Created:	13 Sep 1988
 * $Log: for_user.c,v $
 * Revision 8.0  1989/03/31 15:22:36  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  89/03/31  15:22:36  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/03/31  15:22:36  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/03/31  15:22:36  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  89/03/31  15:22:36  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  89/03/31  15:22:36  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  89/03/31  15:22:36  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.3  89/03/31  15:22:36  dickey
 *		if we are not running in set-uid mode, don't do the fork.
 *		
 *		Revision 1.2  88/09/13  09:45:43  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	For a set-uid program, invokes a function (presumably a system
 *		call), which sets 'errno' iff it finds an error.
 *
 *		By forking, we can reset our uid to the caller's, which is
 *		faster than an exec.
 *
 * Returns:	-1 if an error was found; sets 'errno'.
 */

#include	"ptypes.h"
#include	<errno.h>
extern	int	errno;

#ifdef	SYSTEM5
#else	SYSTEM5
#include	<sys/wait.h>
#define	fork		vfork
#endif	SYSTEM5

for_user(func)
int	(*func)();
{
	register int	count,
			pid;

#ifdef	SYSTEM5
	int		status;
#define	W_RETCODE	((status >> 8) & 0xff)
#else	SYSTEM5
	union	wait	status;
#define	W_RETCODE	status.w_retcode
#endif	SYSTEM5

	if (getuid() == geteuid()) {
		(void)(*func)();	/* invoke the special function */
		return (0);
	}

	if ((pid = fork()) > 0) {
		while ((count = wait(&status)) != pid) {
			if ((count < 0) || (errno == ECHILD))
				break;
			errno = 0;
		}
		if (errno = W_RETCODE)
			return (-1);
		return (0);
	} else if (pid == 0) {
		(void)setuid(getuid());
		errno = 0;		/* ensure that 'errno' is reset */
		(void)(*func)();	/* invoke the special function */
		(void)_exit(errno);	/* return 'errno' to caller */
		/*NOTREACHED*/
	}
	return (-1);
}
