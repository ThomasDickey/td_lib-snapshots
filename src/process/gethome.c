/*
 * Title:	gethome.c (get home name)
 * Author:	T.E.Dickey
 * Created:	06 Jun 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		04 Oct 1989, lint (apollo SR10.1)
 *		
 * Function:	Returns the path of the home directory of the current process.
 */

#define PWD_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: gethome.c,v 12.9 2010/07/05 16:17:46 tom Exp $")

char *
gethome(void)
{
#ifdef	SYS_UNIX
    struct passwd *q;
    const char *r;
    static char *home;

    if (home == 0) {
	if (((q = getpwuid((uid_t) getuid())) == 0)
	    || ((r = q->pw_dir) == 0)) {	/* prefer passwd-file */
	    if ((r = getenv("HOME")) == 0)	/* fall-back (sys-err) */
		r = ".";	/* give up */
	}
	home = stralloc(r);
    }
    return (home);
#endif
#ifdef	vms
    return "sys$login:";
#endif
#ifdef	MSDOS
    return "/";
#endif
}

#ifdef	TEST
_MAIN
{
    (void) argc;
    (void) argv;

    printf("HOME= \"%s\"\n", gethome());
    exit(SUCCESS);
}
#endif /* TEST */
