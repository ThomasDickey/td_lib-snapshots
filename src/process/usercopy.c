#ifndef	lint
static	char	sccs_id[] = "@(#)usercopy.c	1.2 88/08/30 09:28:27";
#endif	lint

/*
 * Title:	usercopy.c (copy a file for setuid program's caller)
 * Author:	T.E.Dickey
 * Created:	30 Aug 1988
 * Modified:
 *
 * Function:	For a set-uid program, copies a file on behalf of the user.
 *		By forking, we can reset our uid; this is faster than an
 *		exec.
 */

#include	"ptypes.h"
#include	<sys/file.h>
#include	<errno.h>
extern	int	errno;

#ifdef	SYSTEM5
#else	SYSTEM5
#include	<sys/wait.h>
#define	fork		vfork
#endif	SYSTEM5

usercopy(src, dst)
char	*src;
char	*dst;
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
		(void)filecopy(src, dst, TRUE);
		(void)_exit(errno);	/* return 'errno' to caller */
		/*NOTREACHED*/
	}
	return (-1);
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
	char	*src, *dst;

	if (argc > 2) {
		printf("src=%s, dst=%s\n", src = argv[1], dst = argv[2]);
		if (usercopy(src, dst) < 0)
			perror(src);
	}
}
#endif	TEST
