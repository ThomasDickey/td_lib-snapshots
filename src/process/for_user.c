/*
 * Title:	for_user.c (carry out function for set-uid user)
 * Author:	T.E.Dickey
 * Created:	13 Sep 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		23 Nov 1992, flush stderr/stdout if we fork
 *		17 Nov 1992, modified _FNX macro.
 *		04 Oct 1991, conversion to ANSI
 *		12 Sep 1991, removed redundant def for 'errno' (VMS C 3.2)
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

MODULE_ID("$Id: for_user.c,v 12.7 2004/03/07 22:03:45 tom Exp $")

#ifdef	SYS_UNIX
int
for_user2(void (*func) (void),
	  int the_uid,
	  int the_gid)
{
    int count, pid;

    DCL_WAIT(status);

    if (the_uid == (int) geteuid()
	&& the_gid == (int) getegid()) {
	errno = 0;		/* ensure that 'errno' is reset */
	(void) (*func) ();	/* invoke the special function */
	return (errno ? -1 : 0);
    }

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
	return (0);
    } else if (pid == 0) {
	(void) setgid(the_gid);	/* try, but don't complain */
	if (setuid(the_uid) < 0)
	    perror("setuid");	/* important to complain here */
	else {
	    errno = 0;		/* ensure that 'errno' is reset */
	    (void) (*func) ();	/* invoke the special function */
	}
	(void) _exit(errno);	/* return 'errno' to caller */
	/*NOTREACHED */
    }
    return (-1);
}

int
for_user(void (*func) (void))
{
    return for_user2(func, (int) getuid(), (int) getgid());
}

#endif /* SYS_UNIX */
