#ifndef	lint
static	char	Id[] = "$Id: execute.c,v 9.1 1991/09/12 07:36:20 dickey Exp $";
#endif

/*
 * Title:	execute.c (execute a command, returning its status)
 * Author:	T.E.Dickey
 * Created:	21 May 1988
 * Modified:
 *		12 Sep 1991, removed redundant def for 'errno' (VMS C 3.2)
 *		31 May 1991, lint (SunOs)
 *		15 May 1991, mods to compile under apollo sr10.3
 *		27 Apr 1990, for vms-port, handle escaped-spaces a la
 *			     'catarg()'.  This makes case8.com for CPROTO work
 *			     correctly.
 *		26 Apr 1990, added ifdef'd code to make this work on VAX/VMS.
 *			     added a test-driver.
 *		24 Apr 1990, flush stdout, stderr before forking to ensure
 *			     that we don't get unnecessarily garbled output!
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
#define	WAI_PTYPES
#include	"ptypes.h"
#include	<ctype.h>
#include	<errno.h>

#if	defined(SYSTEM5) && !defined(vms)
#define	EXECV(c,v,e)	execvp(c,v)
#else	/* bsd4.x or vms */
#ifdef	vms
#include	<descrip.h>
#include	<unixlib.h>
#include	<processes.h>
#else	/* bsd4.x */
extern	char	**environ;
#endif
#define	fork		vfork
#define	EXECV(c,v,e)	execve(c,v,e)
#endif	/* sys5 vs bsd4.x/vms */

	/*ARGSUSED*/
	def_DOALLOC(char *)

execute(verb, args)
char	*verb;
char	*args;
{
	char	cmds[BUFSIZ],
		*s	= strcat(strcat(strcpy(cmds, verb), " "), args);

#ifdef	vms

	/* If this is run on vms, we may be processing a foreign command, or
	 * a standard dcl-command.  In either case, however, both the 'system()'
	 * and 'execv' run-time procedures do not allow stdout/stderr of the
	 * spawned process to write to log-file in batch-mode.  Since the intent
	 * of this procedure is to execute w/o losing or capturing I/O, we use
	 * 'lib$spawn()'.
	 */
	auto	struct	dsc$descriptor_s desc;
	auto	long	st, st2;
	auto	char	tmp[BUFSIZ], *t = tmp;
	auto	int	count	= 0,
			quoted	= FALSE;

	/* Quote mixed-case stuff so that foreign commands work better */
	while (*t = *s) {
		if (*t == '\\' && s[1] == ' ')
			*t = *(++s) | 0200;	/* a la 'catarg()' */
		if (*t == ' ') {
			count++;
			if (quoted) {
				quoted = FALSE;
				*t++ = '"';
				*t = *s;
			}
		} else if (!isascii(*t) || isupper(*t)) {
			if (!quoted && count) {
				quoted = TRUE;
				*t++ = '"';
			}
			*t = toascii(*s);	/* strip catarg-flag */
		}
		s++;
		t++;
	}
	if (quoted) {
		*t++ = '"';
		*t = EOS;
	}

	desc.dsc$w_length = strlen(desc.dsc$a_pointer = tmp);
	desc.dsc$b_dtype  = DSC$K_DTYPE_T;
	desc.dsc$b_class  = DSC$K_CLASS_S;
	st = lib$spawn(&desc, 0,0,0,0,0, &st2);

	if ($VMS_STATUS_SUCCESS(st)
	&&  $VMS_STATUS_SUCCESS(st2))
		return (0);
	errno = EVMSERR;	/* can't do much better than that! */

#else	/* unix */
static	char	**myargv;	/* argument vector for 'bldarg()' */
#ifdef	SYSTEM5
char	*what;
#else	/* !SYSTEM5 */
char	what[BUFSIZ];
#endif	/* SYSTEM5/!SYSTEM5 */
int	count	= 3,		/* minimum needed for 'bldarg()' */
	pid;

	DCL_WAIT(status);

	/* Split the command-string into an argv-like structure suitable for
	 * the 'execv()' procedure:
	 */
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
#else	/* !SYSTEM5 */
	/*
	 * 'execve()' needs an absolute pathname in the first argument.
	 * Use 'which()' to get it.  Note that this won't work for ".",
	 * since I didn't waste time on a getcwd...
	 */
	if (which(what, sizeof(what), *myargv, ".") <= 0) {
		errno = ENOENT;
		return (-1);
	}
#endif	/* SYSTEM5 */

#ifdef	TEST
	dump_exec(what,myargv);
#endif

	(void)fflush(stdout);
	(void)fflush(stderr);
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
		errno = 0;
		(void)EXECV(what, myargv, environ);
		(void)_exit(errno);	/* just in case exec-failed */
		/*NOTREACHED*/
	}
#endif	/* vms/unix */
	return (-1);
}

/************************************************************************
 *	test driver							*
 ************************************************************************/

#ifdef	TEST
static
dump_exec(verb, args)
char	*verb, *args[];
{
	register int	j;
	fprintf(stderr, "-> %s\n", verb);
	for (j = 0; args[j] != 0; j++)
		fprintf(stderr, "%d) %s\n", j, args[j]);
}

main(argc, argv)
char	*argv[];
{
	register int	j;

	fprintf(stderr, "-> %s\n", argv[0]);

	if (argc > 1) {
		char	*verb = argv[1];
		char	parms[BUFSIZ];

		*parms = EOS;
		for (j = 2; j < argc; j++)
			catarg(parms, argv[j]);
		fprintf(stderr, "%% %s %s\n", verb, parms);
		if (execute(verb, parms) >= 0)
			exit(SUCCESS);
		perror(verb);
		exit(FAIL);
	}
	fprintf(stderr, "? expected verb+parms\n");
	exit(FAIL);
}
#endif	/* TEST */
