#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/pathname/RCS/trimpath.c,v 8.0 1989/05/11 12:35:56 ste_cm Rel $";
#endif	lint

/*
 * Title:	trimpath.c
 * Author:	T.E.Dickey
 * Created:	17 Apr 1989 (logic adapted from 'abspath.c')
 *
 * Function:	Convert a pathname into canonical form.
 *
 * Arguments:	path -	buffer on which to make changes
 *		cwd -	value to use for ".", ".." references
 *
 * Returns:	pointer to buffer (modified)
 */
#include	"ptypes.h"
#include	<string.h>

char	*
trimpath(path,cwd)
char	*path;
char	*cwd;
{
	register char *s, *d;

	/*
	 * Convert this to an absolute path somehow
	 */
	if (*path == '/') {
		;
	} else if (*path) {
		char	tmp[BUFSIZ];
		d = strcpy(tmp, cwd);
		s = path;
		if (*s == '.')
			if (s[1] == '\0' || s[1] == '/')
				s++;		/* absorb "." */
		d += strlen(tmp);
		if (d[-1] != '/')		/* add "/" iff we need it */
			(void)strcat(d, "/");
		(void)strcat(d, s);
		(void)strcpy(path,tmp);
	}

	/*
	 * Trim out repeated '/' marks
	 */
	for (s = d = path; *s; s++) {
		if (*s == '/') {
			while (s[1] == '/')
				s++;
		} else if (*s == '.') {
			if (s > path && s[-1] == '/') {
				if (s[1] == '\0')
					break;
				else if (s[1] == '/') {
					s++;
					continue;
				}
			}
		}
		*d++ = *s;
	}

	/*
	 * Trim trailing '/' marks
	 */
	while (d > path+1)
		if (d[-1] == '/')	d--;
		else			break;
	*d = '\0';

	/*
	 * Trim out ".." constructs
	 */
	for (s = path; *s; s++) {
		if (s[0] == '.' && s[1] == '.')
			if ((s > path && s[-1] == '/')
			&&  (s[2] == '\0' || s[2] == '/')) {
				d = s+2;
				if (s > path) {
					s -= 2;
					while (s > path && *s != '/') s--;
					if (s == path &&  !*d) s++;
				} else if (*d)
					s--;
				while (*s++ = *d++);
				s = path;	/* rescan */
			}
	}
	return (path);
}

#ifdef	TEST
do_test(argc,argv)
char	*argv[];
{
	register int j;
	auto	 char	bfr[BUFSIZ];

	for (j = 1; j < argc; j++) {
		(void)trimpath(strcpy(bfr, argv[j]), "/usr/local");
		(void)printf("%d: %s => %s\n", j, argv[j], bfr);
	}
}

main(argc, argv)
char	*argv[];
{
	if (argc > 1)
		do_test(argc, argv);
	else {
		static	char	*tbl[] = {
					"?",
					".",
					"..",
					"../..",
					"./",
					"../",
					"a/b",
					"../a/b",
					"../../../a/b",
					".//.//./tmp"
				};
		do_test(sizeof(tbl)/sizeof(tbl[0]), tbl);
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	TEST
