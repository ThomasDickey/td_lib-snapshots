#ifndef	lint
static	char	Id[] = "$Id: execute.c,v 6.1 1990/04/24 13:31:12 dickey Exp $";
#endif	lint

/*
 * Title:	execute.c (execute a command, returning its status)
 * Author:	T.E.Dickey
 * Created:	21 May 1988
 * $Log: execute.c,v $
 * Revision 6.1  1990/04/24 13:31:12  dickey
 * flush stdout, stderr before forking to ensure that we don't
 * get unnecessarily garbled output!
 *
 *		Revision 6.0  88/08/30  07:10:36  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/08/30  07:10:36  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/08/30  07:10:36  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/08/30  07:10:36  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/30  07:10:36  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.9  88/08/30  07:10:36  dickey
 *		sccs2rcs keywords
 *		
 *		30 Aug 1988, reset 'errno' to ensure proper error reporting.
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

#define	STR_PTYPES
#include	"ptypes.h"
#include	<ctype.h>
#include	<errno.h>
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

	(void)fflush(stdout);
	(void)fflush(stderr);
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
		errno = 0;
		(void)EXECV(what, myargv, environ);
		(void)_exit(errno);	/* just in case exec-failed */
		/*NOTREACHED*/
	}
	return (-1);
}
