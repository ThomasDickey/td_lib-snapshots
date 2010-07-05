/*
 * Title:	usercopy.c (copy a file for setuid program's caller)
 * Author:	T.E.Dickey
 * Created:	30 Aug 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		13 Sep 1988, rewrote using 'for_user()'
 *
 * Function:	For a set-uid program, copies a file on behalf of the user.
 *		By forking, we can reset our uid; this is faster than an
 *		exec.
 */

#include	"ptypes.h"

MODULE_ID("$Id: usercopy.c,v 12.7 2010/07/04 18:18:04 tom Exp $")

#ifdef	SYS_UNIX

static const char *ucp_src, *ucp_dst;

static void
ucp_func(void)
{
    (void) filecopy(ucp_src, ucp_dst, TRUE);
}

int
usercopy(const char *src,
	 const char *dst)
{
    ucp_src = src;
    ucp_dst = dst;
    return (for_user(ucp_func));
}

#ifdef	TEST
_MAIN
{
    char *src, *dst;

    if (argc > 2) {
	printf("src=%s, dst=%s\n", src = argv[1], dst = argv[2]);
	if (usercopy(src, dst) < 0)
	    perror(src);
    }
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif /* TEST */

#endif /* SYS_UNIX */
