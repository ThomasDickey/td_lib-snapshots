#ifndef	lint
static	char	Id[] = "$Id: which.c,v 9.0 1991/05/15 09:58:56 ste_cm Rel $";
#endif

/*
 * Title:	which.c (which-executable scanner)
 * Author:	T.E.Dickey
 * Created:	18 Nov 1987
 * $Log: which.c,v $
 * Revision 9.0  1991/05/15 09:58:56  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:58:56  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  90/03/12  09:04:38  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  90/03/12  09:04:38  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  90/03/12  09:04:38  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.3  90/03/12  09:04:38  dickey
 *		lint (apollo sr10.1)
 *		
 *		Revision 5.2  89/12/11  13:14:38  dickey
 *		lint
 *		
 *		Revision 5.1  89/12/11  09:39:19  dickey
 *		corrected handling to relative pathnames
 *		
 *		Revision 5.0  89/09/28  10:48:21  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.3  89/09/28  10:48:21  dickey
 *		lint (apollo SR10.1)
 *		
 *		Revision 4.2  89/09/06  15:32:08  dickey
 *		use 'getwd()' definition from "ptypes.h"
 *		
 *		Revision 4.1  89/08/25  09:50:40  dickey
 *		force $PATH to be nonnull, in case this is invoked from a
 *		non-unix environment!
 *		
 *		Revision 4.0  88/05/16  13:17:20  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
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
extern	char	*getenv();
extern	char	*pathcat();

static
executable(name)
char	*name;
{
struct	stat	sb;
	if (access(name, X_OK) >= 0) {
		if (stat(name, &sb) >= 0) {
			if ((sb.st_mode & S_IFMT) == S_IFREG)
				return(TRUE);
		}
	}
	return(FALSE);
}

which(bfr, len, find, dot)
char	*bfr;
int	len;
char	*find;
char	*dot;
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
main(argc,argv)
char	*argv[];
{
int	j;
char	bfr[BUFSIZ],
	dot[BUFSIZ];

	(void)getwd(dot);
	for (j = 1; j < argc; j++) {
		which(bfr, sizeof(bfr), argv[j], dot);
		printf("%d '%s' = '%s'\n", j, argv[j], bfr);
	}
}
#endif
