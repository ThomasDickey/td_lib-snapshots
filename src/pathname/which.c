/*
 * Title:	which.c (which-executable scanner)
 * Author:	T.E.Dickey
 * Created:	18 Nov 1987
 * Modified:
 *		04 Dec 1993, port to MSDOS.
 *		29 Oct 1993, ifdef-ident
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

MODULE_ID("$Id: which.c,v 12.9 2001/05/15 00:59:14 tom Exp $")

#ifdef MSDOS
#define PROG_EXTS "PIF", "BAT", "EXE", "COM"
#endif

#if defined(__EMX__) || defined(__CYGWIN32__) || defined(__CYGWIN__)
#define PROG_EXTS "EXE"
#endif

static
int	executable(
	_AR1(char *,	name))
	_DCL(char *,	name)
{
#ifdef	PROG_EXTS
	char	*s = ftype(name);
	char	*t = s;
	int	ok = FALSE;
	register int j;

	if (s == name || s[-1] != '.') {
		*s++ = '.';
		*s = EOS;
	}
	if (s == t) {		/* don't supply a suffix */
		ok = (access(name, R_OK) >= 0);
	} else {
		static	char	*exectypes[] = { PROG_EXTS };
		for (j = 0; j < SIZEOF(exectypes); j++) {
			strcpy(s, exectypes[j]);
			if (access(name, R_OK) >= 0) {
				ok = TRUE;
				break;
			}
		}
	}
	return ok;
#else	/* SYS_UNIX */
	Stat_t	sb;
	return (access(name, X_OK) >= 0) && (stat_file(name, &sb) >= 0);
#endif
}

int	which(
	_ARX(char *,	bfr)
	_ARX(unsigned,	len)
	_ARX(char *,	find)
	_AR1(char *,	dot)
		)
	_DCL(char *,	bfr)
	_DCL(unsigned,	len)
	_DCL(char *,	find)
	_DCL(char *,	dot)
{
	register char *s, *d;
	char	*path = getenv("PATH");
	char	test[BUFSIZ];

	if (path == 0) path = ".";
	s = path;
	*test = *bfr = EOS;

	if (fleaf_delim(find) != 0) {
		if (executable(find))
			(void)pathcat(test, dot, find);
	} else {
#ifdef	MSDOS	/* "." is at the beginning of the implied path */
		if (executable(pathcat(test, dot, find)))
			;
		else
#endif
		while (*s) {
			size_t n;
			for (n = 0; s[n] != EOS && s[n] != PATHLIST_SEP; n++)
				;
			d = s + n;
			if ((n == 0) || !strncmp(s, ".", n)) {
				(void)strcpy(test, dot);
			} else {
				strncpy(test, s, n)[n] = EOS;
			}
			abspath(test);
			(void)pathcat(test, test, find);
			if (executable(test))
				break;
			for (s = d; (*s != EOS) && (*s == PATHLIST_SEP); s++)
				;
			*test = EOS;
		}
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
#endif	/* TEST */
