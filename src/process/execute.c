#ifndef	lint
static	char	sccs_id[] = "@(#)execute.c	1.2 88/06/10 08:23:52";
#endif	lint

/*
 * Title:	execute.c (execute a command, returning its status)
 * Author:	T.E.Dickey
 * Created:	21 May 1988
 * Modified:
 *
 * Function:	Execute a subprocess using an 'execvp()'.  This is faster than
 *		a 'system()' call, and should be used instead of 'system()'
 *		when the process's exit-code is of interest.  A "-1" is
 *		returned on any error.
 */

#include	<stdio.h>
#include	<ctype.h>
extern	char	*doalloc();
extern	char	*strcat();
extern	char	*strcpy();
extern	int	errno;

#ifdef	lint
#define	DOALLOC(c,p,n)	(c *)0
#else	lint
#define	DOALLOC(c,p,n)	(c *)doalloc(p, n * sizeof(c))
#endif	lint

execute(verb, args)
char	*verb;
char	*args;
{
static	char	**myargv;	/* argument vector for 'bldarg()' */
char	cmds[BUFSIZ],
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

	if ((pid = fork()) > 0) {
		while (wait(&status) >= 0);
		if (errno = ((status >> 8) & 0xff))
			return (-1);
		return (0);
	} else if (pid == 0) {
		(void)execvp(*myargv, myargv);
		(void)exit(errno);	/* just in case exec-failed */
		/*NOTREACHED*/
	}
	return (-1);
}
