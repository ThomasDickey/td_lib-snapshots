/*
 * Title:	execute.c (execute a command, returning its status)
 * Author:	T.E.Dickey
 * Created:	21 May 1988
 * Modified:
 *		21 May 1994, mods for 'autoconf'
 *		04 Dec 1993, port to TurboC
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		19 Nov 1992, memory-leak
 *		04 Oct 1991, conversion to ANSI
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

MODULE_ID("$Id: execute.c,v 12.7 1994/05/30 21:19:52 tom Exp $")

#ifdef	vms
#  include	<descrip.h>
#  include	<unixlib.h>
#  include	<processes.h>
#else	/* bsd4.x */
#  if HAVE_EXECVP
#    define	EXECV(c,v,e)	execvp(c,v)
#  else		/* assume bsd4.x */
     extern	char	**environ;
#    define	EXECV(c,v,e)	execve(c,v,e)
#  endif
#endif	/* sys5 vs bsd4.x/vms */

#ifdef	__TURBOC__
#include <process.h>
#endif

	/*ARGSUSED*/
	def_DOALLOC(char *)

#ifdef	TEST
static	void	dump_exec(
		_arx(char *,	verb)
		_ar1(char **,	args));
#endif

int	execute(
	_ARX(char *,	verb)
	_AR1(char *,	args)
		)
	_DCL(char *,	verb)
	_DCL(char *,	args)
{
	/* patch: dyn_string? */
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

#endif	/* vms */
#ifdef	unix
	static	char	**myargv;	/* argument vector for 'bldarg()' */
#if HAVE_EXECVP
	auto	char	*what;
#else
	auto	char	what[BUFSIZ];
#endif	/* HAVE_EXECVP */
	auto	int	count	= 3,	/* minimum needed for 'bldarg()' */
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

#if HAVE_EXECVP
	what = *myargv;
#else
	/*
	 * 'execve()' needs an absolute pathname in the first argument.
	 * Use 'which()' to get it.  Note that this won't work for ".",
	 * since I didn't waste time on a getcwd...
	 */
	if (which(what, sizeof(what), *myargv, ".") <= 0) {
		errno = ENOENT;
		return (-1);
	}
#endif	/* HAVE_EXECVP */

#ifdef	TEST
	dump_exec(what,myargv);
#endif

	FFLUSH(stdout);
	FFLUSH(stderr);
	if ((pid = fork()) > 0) {
		while ((count = wait(ARG_WAIT(status))) != pid) {
			if ((count < 0) || (errno == ECHILD))
				break;
			errno = 0;
		}
		if ((errno = W_RETCODE(status)) != 0)
			return (-1);
#ifdef	NO_LEAKS
		dofree((char *)myargv);
		myargv = 0;
#endif	/*NO_LEAKS*/
	} else if (pid == 0) {
		errno = 0;
		(void)EXECV(what, myargv, environ);
		(void)_exit(errno);	/* just in case exec-failed */
		/*NOTREACHED*/
	}
	return (0);
#endif	/* unix */
	/*
	 * TurboC 3.0 for MS-DOS doesn't have 'fork()' or 'wait()', but it
	 * does provide a similar construct that lets us get the child's
	 * exit status (the whole point of this module).
	 */
#if	defined(__TURBOC__)
	static	char	**myargv;	/* argument vector for 'bldarg()' */
	auto	int	count	= 3;	/* minimum needed for 'bldarg()' */

	/* Split the command-string into an argv-like structure suitable for
	 * the 'spawnv()' procedure:
	 */
	while (*s != EOS) {
		if (isspace(*s)) {
			count++;
			while (isspace(*s))
				s++;
		} else
			s++;
	}
	myargv = DOALLOC(myargv, char *, (unsigned)count);
	bldarg(count, myargv, cmds);

#ifdef	TEST
	dump_exec(*myargv,myargv);
#endif
	FFLUSH(stdout);
	FFLUSH(stderr);
	/* patch: NO_LEAK(myargv) */
	return spawnvp(P_WAIT, *myargv, myargv);
#endif	/* __TURBOC__ */
}

/************************************************************************
 *	test driver							*
 ************************************************************************/

#ifdef	TEST
static
void	dump_exec(
	_ARX(char *,	verb)
	_AR1(char **,	args)
		)
	_DCL(char *,	verb)
	_DCL(char **,	args)
{
	register int	j;
	FPRINTF(stderr, "-> %s\n", verb);
	for (j = 0; args[j] != 0; j++)
		FPRINTF(stderr, "%d) %s\n", j, args[j]);
}

_MAIN
{
	register int	j;

	FPRINTF(stderr, "-> %s\n", argv[0]);

	if (argc > 1) {
		char	*verb = argv[1];
		char	parms[BUFSIZ];

		*parms = EOS;
		for (j = 2; j < argc; j++)
			catarg(parms, argv[j]);
		FPRINTF(stderr, "%% %s %s\n", verb, parms);
		if (execute(verb, parms) < 0) {
			perror(verb);
			exit(FAIL);
		}
		exit(SUCCESS);
	}
	FPRINTF(stderr, "? expected verb+parms\n");
	exit(FAIL);
	/*NOTREACHED*/
}
#endif	/* TEST */
