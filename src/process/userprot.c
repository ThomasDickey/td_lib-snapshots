#ifndef	lint
static	char	sccs_id[] = "@(#)userprot.c	1.1 88/08/30 15:31:34";
#endif	lint

/*
 * Title:	userprot.c (set protection for setuid program's caller)
 * Author:	T.E.Dickey
 * Created:	30 Aug 1988
 * Modified:
 *
 * Function:	For a set-uid program, sets protection (and modification time)
 *		for the user.  By forking, we can reset our uid; this is
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

userprot(name, mode, mtime)
char	*name;
int	mode;
time_t	mtime;
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
		errno = 0;
		if (chmod(name, mode) >= 0)
			(void)setmtime(name, mtime);
		(void)_exit(errno);	/* return 'errno' to caller */
		/*NOTREACHED*/
	}
	return (-1);
}
