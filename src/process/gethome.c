#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/process/RCS/gethome.c,v 4.0 1988/08/10 10:47:21 ste_cm Rel $";
#endif	lint

/*
 * Title:	gethome.c (get home name)
 * Author:	T.E.Dickey
 * Created:	06 Jun 1988
 * $Log: gethome.c,v $
 * Revision 4.0  1988/08/10 10:47:21  ste_cm
 * BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
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

#include	<stdio.h>
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
		if (!(q = getpwuid(getuid()))
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
