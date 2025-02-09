/*
 * Title:	execute.c (execute a command, returning its status)
 * Author:	T.E.Dickey
 * Created:	21 May 1988
 * Modified:
 *		01 Dec 2019, added executev(), to deprecate execute().
 *		12 Dec 2014, fix memory leak (coverity).
 *		07 Mar 2004, remove K&R support, indent'd.
 *		24 Dec 2000, ctype.h fix for QNX
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

#define	CHR_PTYPES
#define	STR_PTYPES
#define	WAI_PTYPES
#include	"ptypes.h"
#include	"dyn_str.h"
#include	<errno.h>

MODULE_ID("$Id: execute.c,v 12.16 2025/01/07 00:16:50 tom Exp $")

#ifdef	vms
#  include	<descrip.h>
#  include	<unixlib.h>
#  include	<processes.h>
#else /* unix */
#endif /* unix vs vms */

#ifdef	__TURBOC__
#include <process.h>
#endif

#ifdef	TEST
static void dump_exec(char *verb, char **args);
#endif

#ifdef SYS_UNIX
int
executev(char **argv)
{
    int result;
    int count;
    int pid;
    DCL_WAIT(status);

#ifdef TEST
    dump_exec(argv[0], argv);
#endif

    FFLUSH(stdout);
    FFLUSH(stderr);
    if ((pid = fork()) > 0) {
	while ((count = wait(ARG_WAIT(status))) != pid) {
	    if ((count < 0) || (errno == ECHILD))
		break;
	    errno = 0;
	}
	if ((errno = W_RETCODE(status)) != 0) {
	    return (-1);
	}
    } else if (pid == 0) {
	errno = 0;
	(void) execvp(argv[0], argv);
	(void) _exit(errno);	/* just in case exec-failed */
	/*NOTREACHED */
    }
    result = 0;
    return result;
}
#endif /* SYS_UNIX */

int
execute(const char *verb, const char *args)
{
    int result;
    size_t need = strlen(verb) + strlen(args) + 2;
    DYN *dyn = NULL;
    char *s = strcat(
			strcat(
				  strcpy((dyn = dyn_alloc((DYN *) 0,
							  need))->text,
					 verb), " "), args);
#if defined(vms)

    /* If this is run on vms, we may be processing a foreign command, or
     * a standard dcl-command.  In either case, however, both the 'system()'
     * and 'execv' run-time procedures do not allow stdout/stderr of the
     * spawned process to write to log-file in batch-mode.  Since the intent
     * of this procedure is to execute w/o losing or capturing I/O, we use
     * 'lib$spawn()'.
     */
    struct dsc$descriptor_s desc;
    long st, st2;
    char tmp[BUFSIZ], *t = tmp;
    int count = 0, quoted = FALSE;

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
    desc.dsc$b_dtype = DSC$K_DTYPE_T;
    desc.dsc$b_class = DSC$K_CLASS_S;
    st = lib$spawn(&desc, 0, 0, 0, 0, 0, &st2);

    if ($VMS_STATUS_SUCCESS(st)
	&& $VMS_STATUS_SUCCESS(st2)) {
	dyn_free(dyn);
	return (0);
    }
    errno = EVMSERR;		/* can't do much better than that! */

#elif defined(SYS_UNIX)
    static char **myargv;	/* argument vector for 'bldarg()' */
    int count = 3;		/* minimum needed for 'bldarg()' */

    /* Split the command-string into an argv-like structure suitable for
     * the 'execv()' procedure:
     */
    while (*s) {
	if (isspace(UCH(*s))) {
	    count++;
	    while (isspace(UCH(*s)))
		s++;
	} else {
	    s++;
	}
    }
    myargv = DOALLOC(myargv, char *, (unsigned) count);
    bldarg(count, myargv, dyn->text);

    result = executev(myargv);
#elif defined(__TURBOC__)
    /*
     * TurboC 3.0 for MS-DOS doesn't have 'fork()' or 'wait()', but it
     * does provide a similar construct that lets us get the child's
     * exit status (the whole point of this module).
     */
    static char **myargv;	/* argument vector for 'bldarg()' */
    int count = 3;		/* minimum needed for 'bldarg()' */

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
    myargv = DOALLOC(myargv, char *, (unsigned) count);
    bldarg(count, myargv, p->text);

#ifdef	TEST
    dump_exec(*myargv, myargv);
#endif
    FFLUSH(stdout);
    FFLUSH(stderr);
    /* patch: NO_LEAK(myargv) */
    result = spawnvp(P_WAIT, *myargv, myargv);
#endif /* __TURBOC__ */
    dyn_free(dyn);
    return result;
}

/************************************************************************
 *	test driver							*
 ************************************************************************/

#ifdef	TEST
static void
dump_exec(char *verb, char **args)
{
    int j;
    FPRINTF(stderr, "-> %s\n", verb);
    for (j = 0; args[j] != 0; j++)
	FPRINTF(stderr, "%d) %s\n", j, args[j]);
}

_MAIN
{
    int j;

    FPRINTF(stderr, "-> %s\n", argv[0]);

    if (argc > 1) {
	char *verb = argv[1];
	char parms[BUFSIZ];

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
    /*NOTREACHED */
}
#endif /* TEST */
