#ifndef	lint
static	char	Id[] = "$Id: usercopy.c,v 12.0 1991/10/04 08:00:51 ste_cm Rel $";
#endif

/*
 * Title:	usercopy.c (copy a file for setuid program's caller)
 * Author:	T.E.Dickey
 * Created:	30 Aug 1988
 * Modified:
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		13 Sep 1988, rewrote using 'for_user()'
 *
 * Function:	For a set-uid program, copies a file on behalf of the user.
 *		By forking, we can reset our uid; this is faster than an
 *		exec.
 */

#include	"ptypes.h"

static	char	*ucp_src,
		*ucp_dst;
static		ucp_func(_AR0) { (void)filecopy(ucp_src, ucp_dst, TRUE); }

usercopy(
_ARX(char *,	src)
_AR1(char *,	dst)
	)
_DCL(char *,	src)
_DCL(char *,	dst)
{
	ucp_src = src;
	ucp_dst = dst;
	return (for_user(ucp_func));
}

#ifdef	TEST
_MAIN
{
	char	*src, *dst;

	if (argc > 2) {
		printf("src=%s, dst=%s\n", src = argv[1], dst = argv[2]);
		if (usercopy(src, dst) < 0)
			perror(src);
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
