#ifndef	lint
static	char	Id[] = "$Id: usercopy.c,v 9.0 1991/05/15 09:57:09 ste_cm Rel $";
#endif

/*
 * Title:	usercopy.c (copy a file for setuid program's caller)
 * Author:	T.E.Dickey
 * Created:	30 Aug 1988
 * $Log: usercopy.c,v $
 * Revision 9.0  1991/05/15 09:57:09  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:57:09  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  88/09/13  09:51:37  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  88/09/13  09:51:37  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/09/13  09:51:37  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/09/13  09:51:37  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/09/13  09:51:37  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/09/13  09:51:37  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/09/13  09:51:37  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.4  88/09/13  09:51:37  dickey
 *		sccs2rcs keywords
 *		
 *		13 Sep 1988, rewrote using 'for_user()'
 *
 * Function:	For a set-uid program, copies a file on behalf of the user.
 *		By forking, we can reset our uid; this is faster than an
 *		exec.
 */

#include	"ptypes.h"

static	char	*ucp_src,
		*ucp_dst;
static		ucp_func() { (void)filecopy(ucp_src, ucp_dst, TRUE); }

usercopy(src, dst)
char	*src;
char	*dst;
{
	ucp_src = src;
	ucp_dst = dst;
	return (for_user(ucp_func));
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
	char	*src, *dst;

	if (argc > 2) {
		printf("src=%s, dst=%s\n", src = argv[1], dst = argv[2]);
		if (usercopy(src, dst) < 0)
			perror(src);
	}
}
#endif
