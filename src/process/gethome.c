#ifndef	lint
static	char	Id[] = "$Id: gethome.c,v 8.0 1989/10/04 13:11:22 ste_cm Rel $";
#endif	lint

/*
 * Title:	gethome.c (get home name)
 * Author:	T.E.Dickey
 * Created:	06 Jun 1988
 * $Log: gethome.c,v $
 * Revision 8.0  1989/10/04 13:11:22  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  89/10/04  13:11:22  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/10/04  13:11:22  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/10/04  13:11:22  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.1  89/10/04  13:11:22  dickey
 *		lint (apollo SR10.1)
 *		
 *		Revision 4.0  88/08/10  10:47:21  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/08/10  10:47:21  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/10  10:47:21  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.3  88/08/10  10:47:21  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Returns the path of the home directory of the current process.
 */

#include	"ptypes.h"
#include <pwd.h>
extern	char	*getenv();
extern	char	*stralloc();

char *
gethome()
{
	extern	 struct passwd *getpwuid();		/* cf: apollo sys5 */
	register struct passwd *q;
	register char	*r;
	static   char	*home;

	if (home == 0) {
		if (!(q = getpwuid((int)getuid()))
		|| (!(r = q->pw_dir))) {		/* prefer passwd-file */
			if (!(r = getenv("HOME")))	/* fall-back (sys-err)*/
				r = ".";		/* give up */
		}
		home = stralloc(r);
	}
	return (home);
}

#ifdef	TEST
main()
{
	printf("HOME= \"%s\"\n", gethome());
}

failed(s) char *s; { perror(s); exit(1); }
#endif	TEST
