#ifndef	lint
static	char	sccs_id[] = "@(#)gethome.c	1.1 88/06/07 07:08:12";
#endif	lint

/*
 * Title:	gethome.c (get home name)
 * Author:	T.E.Dickey
 * Created:	06 Jun 1988
 * Modified:
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
