#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/process/RCS/makeargv.c,v 8.0 1989/04/19 09:42:32 ste_cm Rel $";
#endif	lint

/*
 * Title:	makeargv.c
 * Author:	T.E.Dickey
 * Created:	14 Apr 1989
 *
 * Function:	splits a string into argv-like array of strings.  The maximum
 *		number of strings is fixed (for simplicity).  Single-character
 *		quotes are permitted to preserve blanks.
 *
 *		Unlike normal unix argv, this is not terminated by a null-
 *		pointer; unused strings are set to zero-length constant strings.
 */
#include <ctype.h>

makeargv(argv, maxarg, dst, src)
char	**argv;
char	*dst, *src;
{
	auto	int	argc;

	for (argc = 1; argc < maxarg; argv[argc++] = "");
	argv[argc = 0] = dst;
	argc++;

	while (*dst = *src++) {
		if (isspace(*dst)) {
			*dst++ = '\0';
			while (isspace(*src))	src++;
			if (argc >= maxarg)
				break;
			argv[argc++] = dst;
		} else if (*dst == '"' || *dst == '\'') {
			int	delim = *dst;
			while (*dst = *src++) {
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
main()
{
	int	j,k;
	char	*vec[NUM];
	char	tmp[80];
	static	char	*tbl[] = {
			"abc",
			"a bz cx dy w",
			"ab ' blank 'appended junk",
			"ab\"quoted' 'quote\" more stuff"
		};

	for (j = 0; j < sizeof(tbl)/sizeof(tbl[0]); j++) {
		int	last = makeargv(vec, NUM, tmp,tbl[j]);
		printf("%s\n", tbl[j]);
		for (k = 0; k < last; k++)
			printf("%d:\t'%s'\n", k, p[k]);
	}
}
#endif	TEST