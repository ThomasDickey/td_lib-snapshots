#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/process/RCS/for_user.c,v 1.2 1988/09/13 09:45:43 dickey Exp $";
#endif	lint

/*
 * Title:	for_user.c (carry out function for set-uid user)
 * Author:	T.E.Dickey
 * Created:	13 Sep 1988
 * $Log: for_user.c,v $
 * Revision 1.2  1988/09/13 09:45:43  dickey
 * sccs2rcs keywords
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
