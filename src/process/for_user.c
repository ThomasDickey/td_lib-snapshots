#ifndef	lint
static	char	Id[] = "$Id: for_user.c,v 9.2 1991/06/25 13:25:35 ste_cm Exp $";
#endif

/*
 * Title:	for_user.c (carry out function for set-uid user)
 * Author:	T.E.Dickey
 * Created:	13 Sep 1988
 * Modified:
 *		25 Jun 1991, added entrypoint 'for_user2()'. Ensure that we
 *			     reset group-id, and properly set 'errno' when
 *			     we did not need to fork.
 *		31 Mar 1989, if we are not running in set-uid mode, don't do
 *			     the fork.
 *		
 * Function:	For a set-uid program, invokes a function (presumably a system
 *		call), which sets 'errno' iff it finds an error.
 *
 *		By forking, we can reset our uid to the caller's, which is
 *		faster than running a program via an exec.
 *
 * Returns:	-1 if an error was found; sets 'errno'.
 */

#define	WAI_PTYPES
#include	"ptypes.h"
#include	<errno.h>
extern	int	errno;

for_user2(func, the_uid, the_gid)
int	(*func)();
int	the_uid, the_gid;
{
	register int	count,
			pid;

	DCL_WAIT(status);

	if (the_uid == geteuid()
	&&  the_gid == getegid()) {
		errno = 0;		/* ensure that 'errno' is reset */
		(void)(*func)();	/* invoke the special function */
		return (errno ? -1 : 0);
	}

	if ((pid = fork()) > 0) {
		while ((count = wait(ARG_WAIT(status))) != pid) {
			if ((count < 0) || (errno == ECHILD))
				break;
			errno = 0;
		}
		if (errno = W_RETCODE(status))
			return (-1);
		return (0);
	} else if (pid == 0) {
		(void)setgid(the_gid);	/* try, but don't complain */
		if (setuid(the_uid) < 0)
			perror("setuid"); /* important to complain here */
		else {
			errno = 0;	/* ensure that 'errno' is reset */
			(void)(*func)();/* invoke the special function */
		}
		(void)_exit(errno);	/* return 'errno' to caller */
		/*NOTREACHED*/
	}
	return (-1);
}

for_user(func)
int	(*func)();
{
	return for_user2(func, getuid(), getgid());
}
