#ifndef	lint
static	char	sccs_id[] = "@(#)execute.c	1.3 88/07/27 10:43:35";
#endif	lint

/*
 * Title:	execute.c (execute a command, returning its status)
 * Author:	T.E.Dickey
 * Created:	21 May 1988
 * Modified:
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
extern	char	*doalloc();
extern	char	*strcat();
extern	char	*strcpy();
extern	char	**environ;
extern	int	errno;

#ifdef	lint
#define	DOALLOC(c,p,n)	(c *)0
#else	lint
#define	DOALLOC(c,p,n)	(c *)doalloc(p, n * sizeof(c))
#endif	lint

#ifndef	SYSTEM5
#define	fork	vfork
#define	execvp	execve
#endif	SYSTEM5

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
	pid,
	status;

	while (*s) {
		if (isspace(*s)) {
			count++;
			while (isspace(*s))
				s++;
		} else
			s++;
	}
	myargv = DOALLOC(char *, myargv, count);
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
		while (wait(&status) >= 0);
		if (errno = ((status >> 8) & 0xff))
			return (-1);
		return (0);
	} else if (pid == 0) {
		(void)execvp(what, myargv, environ);
		(void)_exit(errno);	/* just in case exec-failed */
		/*NOTREACHED*/
	}
	return (-1);
}
