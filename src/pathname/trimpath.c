#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: trimpath.c,v 12.2 1993/10/29 17:35:23 dickey Exp $";
#endif

/*
 * Title:	trimpath.c
 * Author:	T.E.Dickey
 * Created:	17 Apr 1989 (logic adapted from 'abspath.c')
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	Convert a pathname into canonical form.
 *
 * Arguments:	path -	buffer on which to make changes
 *		cwd -	value to use for ".", ".." references
 *
 * Returns:	pointer to buffer (modified)
 */

#define	STR_PTYPES
#include	"ptypes.h"

char	*
trimpath(
_ARX(char *,	path)
_AR1(char *,	cwd)
	)
_DCL(char *,	path)
_DCL(char *,	cwd)
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
			if (s[1] == EOS || s[1] == '/')
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
				if (s[1] == EOS)
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
	*d = EOS;

	/*
	 * Trim out ".." constructs
	 */
	for (s = path; *s; s++) {
		if (s[0] == '.' && s[1] == '.')
			if ((s > path && s[-1] == '/')
			&&  (s[2] == EOS || s[2] == '/')) {
				d = s+2;
				if (s > path) {
					s -= 2;
					while (s > path && *s != '/') s--;
					if (s == path &&  !*d) s++;
				} else if (*d)
					s--;
				while ((*s++ = *d++) != EOS)
					;
				s = path;	/* rescan */
			}
	}
	return (path);
}

#ifdef	TEST
void	do_test(
	_arx(int,	argc)
	_ar1(char **,	argv));

void	do_test(
	_ARX(int,	argc)
	_AR1(char **,	argv)
		)
	_DCL(int,	argc)
	_DCL(char **,	argv)
{
	register int j;
	auto	 char	bfr[BUFSIZ];

	for (j = 1; j < argc; j++) {
		(void)trimpath(strcpy(bfr, argv[j]), "/usr/local");
		PRINTF("%d: %s => %s\n", j, argv[j], bfr);
	}
}

_MAIN
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
		do_test(SIZEOF(tbl), tbl);
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
