/*
 * Title:	for_admin.c (carry out function for set-uid effective-user)
 * Author:	T.E.Dickey
 * Created:	11 Oct 1991 (from 'for_user.c')
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		23 Nov 1992, flush stderr/stdout if we fork
 *		17 Nov 1992, modified _FNX macro.
 *		
 * Function:	For a set-uid program, invokes a function (presumably a system
 *		call), which sets 'errno' iff it finds an error.
 *
 *		By forking, we can reset our real-uid (and be able to restore
 *		our original effective+real uid), which is faster than running
 *		a program via an exec.
 *
 * Returns:	-1 if an error was found; sets 'errno'.
 */

#define	WAI_PTYPES
#include	"ptypes.h"
#include	<errno.h>

MODULE_ID("$Id: for_admn.c,v 12.8 2010/07/04 13:14:21 tom Exp $")

#ifdef	SYS_UNIX
int
for_admin2(int (*func) (void),
	   uid_t the_uid,
	   gid_t the_gid)
{
    int count, pid;

    DCL_WAIT(status);

    if (the_uid == getuid()
	&& the_gid == getgid()) {
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
for_admin(int (*func) (void))
{
    return for_admin2(func, geteuid(), getegid());
}

#endif /* SYS_UNIX */
