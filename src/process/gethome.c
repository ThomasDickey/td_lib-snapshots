#ifndef	lint
static	char	Id[] = "$Id: gethome.c,v 12.1 1993/09/21 18:54:04 dickey Exp $";
#endif

/*
 * Title:	gethome.c (get home name)
 * Author:	T.E.Dickey
 * Created:	06 Jun 1988
 * Modified:
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		04 Oct 1989, lint (apollo SR10.1)
 *		
 * Function:	Returns the path of the home directory of the current process.
 */

#define PWD_PTYPES
#include	"ptypes.h"

char *
gethome(_AR0)
{
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
_MAIN
{
	printf("HOME= \"%s\"\n", gethome());
	exit(SUCCESS);
}
#endif
