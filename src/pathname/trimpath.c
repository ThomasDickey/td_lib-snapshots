/*
 * Title:	trimpath.c
 * Author:	T.E.Dickey
 * Created:	17 Apr 1989 (logic adapted from 'abspath.c')
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
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

MODULE_ID("$Id: trimpath.c,v 12.6 2004/03/07 22:03:45 tom Exp $")

char *
trimpath(char *path, char *cwd)
{
    static char slash[] =
    {PATH_SLASH, EOS};
    char *s, *d;

    /*
     * Convert this to an absolute path somehow
     */
    if (isSlash(*path)) {
	;
    } else if (*path) {
	char tmp[BUFSIZ];
	d = strcpy(tmp, cwd);
	s = path;
	if (*s == '.')
	    if (s[1] == EOS || isSlash(s[1]))
		s++;		/* absorb "." */
	d += strlen(tmp);
	if (!isSlash(d[-1]))	/* add delim iff we need it */
	    (void) strcat(d, slash);
	(void) strcat(d, s);
	(void) strcpy(path, tmp);
    }

    /*
     * Trim out repeated path-delimiter marks
     */
    for (s = d = path; *s; s++) {
	if (isSlash(*s)) {
	    while (isSlash(s[1]))
		s++;
	} else if (*s == '.') {
	    if (s > path && isSlash(s[-1])) {
		if (s[1] == EOS)
		    break;
		else if (isSlash(s[1])) {
		    s++;
		    continue;
		}
	    }
	}
	*d++ = *s;
    }

    /*
     * Trim trailing path-delimiter marks
     */
    while (d > path + 1)
	if (isSlash(d[-1]))
	    d--;
	else
	    break;
    *d = EOS;

    /*
     * Trim out ".." constructs
     */
    for (s = path; *s; s++) {
	if (s[0] == '.' && s[1] == '.')
	    if ((s > path && isSlash(s[-1]))
		&& (s[2] == EOS || isSlash(s[2]))) {
		d = s + 2;
		if (s > path) {
		    s -= 2;
		    while (s > path && !isSlash(*s))
			s--;
		    if (s == path && !*d)
			s++;
		} else if (*d)
		    s--;
		while ((*s++ = *d++) != EOS) ;
		s = path;	/* rescan */
	    }
    }
    return (path);
}

#ifdef	TEST
void do_test(
		_arx(int, argc)
		_ar1(char **, argv));

void
do_test(int argc, char **argv)
{
    int j;
    char bfr[BUFSIZ];

    for (j = 1; j < argc; j++) {
	(void) trimpath(strcpy(bfr, argv[j]), "/usr/local");
	PRINTF("%d: %s => %s\n", j, argv[j], bfr);
    }
}

_MAIN
{
    if (argc > 1)
	do_test(argc, argv);
    else {
	static char *tbl[] =
	{
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
    /*NOTREACHED */
}
#endif /* TEST */
