#ifndef	lint
static	char	Id[] = "$Id: which.c,v 12.1 1993/09/21 18:54:02 dickey Exp $";
#endif

/*
 * Title:	which.c (which-executable scanner)
 * Author:	T.E.Dickey
 * Created:	18 Nov 1987
 * Modified:
 *		21 Sep 1993, gcc-warnings
 *		06 Feb 1992, use 'stat_file()'
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		12 Mar 1990, lint (apollo sr10.1)
 *		11 Dec 1989, lint.  Corrected handling to relative pathnames
 *		28 Sep 1989, lint (apollo SR10.1)
 *		06 Sep 1989, use 'getwd()' definition from "ptypes.h"
 *		25 Aut 1989, force $PATH to be nonnull, in case this is invoked
 *			     from a non-unix environment!
 *		
 * Function:	Perform scanning along the PATH environment variable to
 *		find the first instance of a file 'find' which is
 *		executable.
 *
 * Returns:	The length of the resulting string (0 if any error was
 *		encountered, or if no executable-file was found).
 *
 * Bugs:	Does not look at csh-aliases.
 */

#define	ACC_PTYPES
#define	STR_PTYPES
#include	"ptypes.h"

static
int	executable(
	_AR1(char *,	name))
	_DCL(char *,	name)
{
	STAT	sb;
	return (access(name, X_OK) >= 0) && (stat_file(name, &sb) >= 0);
}

int	which(
	_ARX(char *,	bfr)
	_ARX(int,	len)
	_ARX(char *,	find)
	_AR1(char *,	dot)
		)
	_DCL(char *,	bfr)
	_DCL(int,	len)
	_DCL(char *,	find)
	_DCL(char *,	dot)
{
	register char *s, *d;
	char	*path = getenv("PATH");
	char	test[BUFSIZ];

	if (path == 0) path = ".";
	s = path;
	*test = *bfr = EOS;

	if (strchr(find,'/') != 0) {
		if (executable(find))
			(void)pathcat(test, dot, find);
	} else while (*s) {
		for (d = s; (*d != EOS) && (*d != ':'); d++);
		if ((d == s) || !strncmp(s, ".", (size_t)(d-s)))
			(void)strcpy(test, dot);
		else {
			(void)strncpy(test, s, (size_t)(d-s));
			test[d-s] = EOS;
		}
		abspath(test);
		(void)strcat(strcat(test, "/"), find);
		if (executable(test)) break;
		for (s = d; (*s != EOS) && (*s == ':'); s++);
		*test = EOS;
	}

	if (len > strlen(test))
		(void)strcpy(bfr, test);
	return(strlen(bfr));
}

#ifdef	TEST
_MAIN
{
int	j;
char	bfr[BUFSIZ],
	dot[BUFSIZ];

	(void)getwd(dot);
	for (j = 1; j < argc; j++) {
		which(bfr, sizeof(bfr), argv[j], dot);
		printf("%d '%s' = '%s'\n", j, argv[j], bfr);
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
