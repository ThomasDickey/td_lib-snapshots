/*
 * Title:	makeargv.c
 * Author:	T.E.Dickey
 * Created:	14 Apr 1989
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	splits a string into argv-like array of strings.  The maximum
 *		number of strings is fixed (for simplicity).  Single-character
 *		quotes are permitted to preserve blanks.
 *
 *		Unlike normal unix argv, this is not terminated by a null-
 *		pointer; unused strings are set to zero-length constant strings.
 */

#include "ptypes.h"
#include <ctype.h>

MODULE_ID("$Id: makeargv.c,v 12.4 2002/07/03 13:06:37 tom Exp $")

int	makeargv(
	_ARX(char **,	argv)
	_ARX(int,	maxarg)
	_ARX(char *,	dst)
	_AR1(char *,	src)
		)
	_DCL(char **,	argv)
	_DCL(int,	maxarg)
	_DCL(char *,	dst)
	_DCL(char *,	src)
{
	auto	int	argc;

	for (argc = 1; argc < maxarg; argv[argc++] = "");
	argv[argc = 0] = dst;
	argc++;

	while ((*dst = *src++) != EOS) {
		if (isspace(UCH(*dst))) {
			*dst++ = EOS;
			while (isspace(UCH(*src)))
				src++;
			if (argc >= maxarg)
				break;
			argv[argc++] = dst;
		} else if (*dst == '"' || *dst == '\'') {
			int	delim = *dst;
			while ((*dst = *src++) != EOS) {
				if (*dst == delim)
					break;
				dst++;
			}
		} else
			dst++;
	}
	return (argc);
}

#ifdef	TEST
_MAIN
{
	int	j,k;
	char	*vec[BUFSIZ];
	char	tmp[80];
	static	char	*tbl[] = {
			"abc",
			"a bz cx dy w",
			"ab ' blank 'appended junk",
			"ab\"quoted' 'quote\" more stuff"
		};

	for (j = 0; j < SIZEOF(tbl); j++) {
		int	last = makeargv(vec, BUFSIZ, tmp,tbl[j]);
		printf("%s\n", tbl[j]);
		for (k = 0; k < last; k++)
			printf("%d:\t'%s'\n", k, vec[k]);
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
