#ifndef	lint
static	char	sccs_id[] = "@(#)execute.c	1.7 88/08/18 07:02:18";
#endif	lint

/*
 * Title:	execute.c (execute a command, returning its status)
 * Author:	T.E.Dickey
 * Created:	21 May 1988
 * Modified:
 *		18 Aug 1988, made wait-loop more explicit
 *		27 Jul 1988, on bsd4.x systems, use 'vfork()' and 'execvp()'.
 *			     This is much faster (one the order of a second on
 *			     Apollo DN3000).
 *
 * Function:	Execute a subprocess using an 'execvp()'.  This is faster than
 *		a 'system()' call, and should be used instead of 'system()'
 *		when the process's exit-code is of interest.  A "-1" is
 *		returned on any error.
 */

#include	"ptypes.h"
#include	<stdio.h>
#include	<ctype.h>
#include	<errno.h>
extern	char	*strcat();
extern	char	*strcpy();
extern	int	errno;

#ifdef	SYSTEM5
#define	EXECV(c,v,e)	execvp(c,v)
#else	SYSTEM5
#include	<sys/wait.h>
extern	char	**environ;
#define	fork		vfork
#define	EXECV(c,v,e)	execve(c,v,e)
#endif	SYSTEM5

	/*ARGSUSED*/
	def_DOALLOC(char *)

execute(verb, args)
char	*verb;
char	*args;
{
static	char	**myargv;	/* argument vector for 'bldarg()' */
char	cmds[BUFSIZ],
#ifdef	SYSTEM5
	*what,
#else	SYSTEM5
	what[BUFSIZ],
#endif	SYSTEM5
	*s	= strcat(strcat(strcpy(cmds, verb), " "), args);
int	count	= 3,		/* minimum needed for 'bldarg()' */
	pid;

#ifdef	SYSTEM5
int	status;
#define	W_RETCODE	((status >> 8) & 0xff)
#else	SYSTEM5
union	wait	status;
#define	W_RETCODE	status.w_retcode
#endif	SYSTEM5

	while (*s) {
		if (isspace(*s)) {
			count++;
			while (isspace(*s))
				s++;
		} else
			s++;
	}
	myargv = DOALLOC(myargv, char *, (unsigned)count);
	bldarg(count, myargv, cmds);

#ifdef	SYSTEM5
	what = *myargv;
#else	SYSTEM5
	/*
	 * 'execve()' needs an absolute pathname in the first argument.
	 * Use 'which()' to get it.  Note that this won't work for ".",
	 * since I didn't waste time on a getcwd...
	 */
	if (which(what, sizeof(what), *myargv, ".") <= 0) {
		errno = ENOENT;
		return (-1);
	}
#endif	SYSTEM5

	if ((pid = fork()) > 0) {
		while ((count = wait(&status)) != pid) {
			if ((count < 0) || (errno == ECHILD))
				break;
		}
		if (errno = W_RETCODE)
			return (-1);
		return (0);
	} else if (pid == 0) {
		(void)EXECV(what, myargv, environ);
		(void)_exit(errno);	/* just in case exec-failed */
		/*NOTREACHED*/
	}
	return (-1);
}
