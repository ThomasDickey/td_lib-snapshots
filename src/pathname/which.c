#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/pathname/RCS/which.c,v 4.0 1988/05/16 13:17:20 ste_cm Rel $";
#endif	lint

/*
 * Title:	which.c (which-executable scanner)
 * Author:	T.E.Dickey
 * Created:	18 Nov 1987
 * $Log: which.c,v $
 * Revision 4.0  1988/05/16 13:17:20  ste_cm
 * BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *
 *		Revision 3.0  88/05/16  13:17:20  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/05/16  13:17:20  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.3  88/05/16  13:17:20  dickey
 *		sccs2rcs keywords
 *		
 *		16 May 1988, use 'abspath()' to expand directory names.
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
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
extern	char	*getcwd(),
		*getenv(),
		*strcat(),
		*strcpy(),
		*strncpy();

#define	EOS	'\0'

static
executable(name)
char	*name;
{
struct	stat	sb;
	if (access(name, 1) >= 0) {
		if (stat(name, &sb) >= 0) {
			if ((sb.st_mode & S_IFMT) == S_IFREG)
				return(1);
		}
	}
	return(0);
}

which(bfr, len, find, dot)
char	*bfr;
char	*find;
char	*dot;
{
register char *s, *d;
char	*path = getenv("PATH");
char	test[BUFSIZ];

	if (path == 0) path = "";
	s = path;
	*test = *bfr = EOS;

	if ((*find == '/') && executable(find)) {
		(void)strcpy(test, find);
	} else while (*s) {
		for (d = s; (*d != EOS) && (*d != ':'); d++);
		if ((d == s) || !strncmp(s, ".", d-s))
			(void)strcpy(test, dot);
		else {
			(void)strncpy(test, s, d-s);
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
main(argc,argv)
char	*argv[];
{
int	j;
char	bfr[BUFSIZ],
	dot[BUFSIZ];

	getcwd(dot, sizeof(dot)-2);
	for (j = 1; j < argc; j++) {
		which(bfr, sizeof(bfr), argv[j], dot);
		printf("%d '%s' = '%s'\n", j, argv[j], bfr);
	}
}
#endif	TEST
