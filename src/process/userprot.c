/*
 * Title:	userprot.c (set protection for setuid program's caller)
 * Author:	T.E.Dickey
 * Created:	30 Aug 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		04 Oct 1991, conversion to ANSI
 *		12 Sep 1991, removed redundant def for 'errno' (VMS C 3.2)
 *		11 Jul 1991, if effective-user is root, set ownership and act
 *			     within the current process.
 *		13 Sep 1988, rewrote using 'for_user()'
 *
 * Function:	For a set-uid program, sets protection (and modification time)
 *		for the user.  By forking, we can reset our uid; this is
 *		faster than an exec.
 *
 * Returns:	-1 if an error was found; sets 'errno'.
 */

#include	"ptypes.h"
#include	<errno.h>

MODULE_ID("$Id: userprot.c,v 12.10 2014/12/28 01:11:01 tom Exp $")

#ifdef	SYS_UNIX

static char *upr_name;
static mode_t upr_mode;
static time_t upr_time;

static
void
upr_func(void)
{
    if (chmod(upr_name, upr_mode) >= 0)
	(void) setmtime(upr_name, upr_time, (time_t) 0);
}

int
userprot(
	    char *name,
	    int mode,
	    time_t mtime)
{
    upr_name = name;
    upr_mode = (mode_t) mode;
    upr_time = mtime;

    if (!geteuid()) {
	if (chown(upr_name, getuid(), getgid()) < 0)
	    return (-1);
	errno = 0;
	upr_func();
	return (errno != 0) ? -1 : 0;
    }
    return (for_user(upr_func));
}

#endif /* SYS_UNIX */

/******************************************************************************/
#ifdef	TEST
_MAIN
{
    (void) argc;
    (void) argv;
    exit(EXIT_FAILURE);
    /*NOTREACHED */
}
#endif /* TEST */
