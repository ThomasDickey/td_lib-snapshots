#ifndef	lint
static	char	Id[] = "$Id: relpath.c,v 10.0 1991/10/03 08:46:02 ste_cm Rel $";
#endif

/*
 * Title:	relpath.c (convert path to relative-form)
 * Author:	T.E.Dickey
 * Created:	07 Sep 1989
 * Modified:
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		12 Mar 1990, lint (apollo sr10.1)
 *		
 *
 * Function:	Converts a pathname from (presumably absolute form) to relative
 *		form.
 */

#define	STR_PTYPES
#include	"ptypes.h"

char	*
relpath(
_ARX(char *,	dst)
_ARX(char *,	cwd)
_AR1(char *,	src)
	)
_DCL(char *,	dst)
_DCL(char *,	cwd)
_DCL(char *,	src)
{
	auto	char	current[BUFSIZ];
	auto	char	tmp[BUFSIZ];
	auto	char	pre[BUFSIZ];
	auto	size_t	j;

	src = strcpy(tmp, src);	/* dst may be the same as src; copy it */
	if (cwd == 0)		/* if cwd not given, get the actual path */
		cwd = getwd(current);
	else
		cwd = strcpy(current, cwd);

	if (cwd != 0) {
		(void)strcpy(pre, ".");
		for (;;) {
			if (	!strcmp(cwd,src))
				return (strcpy(dst, pre));
			if (	((j = strlen(cwd)) < strlen(src))
			&&	!strncmp(cwd,src,j)
			&&	src[j] == '/')
				return (pathcat(dst, pre, src+j+1));
			if (strchr(src,'/') == 0) {
				if (dotname(src))
					return (strcpy(dst, src));
				return (pathcat(dst, pre, src));
			}

			/*
			 * Trim off a leaf from current-directory, add a level
			 * of ".." to prefix:
			 */
			(void)strcat(pre, pre[1] ? "/.." : ".");
			if (j > 0 && cwd[--j] != '/') {
				while (cwd[j] != '/')
					cwd[j--] = 0;
				if (j > 0 && cwd[j-1] != '/')
					cwd[j] = 0;
			} else
				break;
		}
	}
	return (strcpy(dst, src));
}

#ifdef	TEST
do_test(
_AR1(char *,	s))
_DCL(char *,	s)
{
	auto	char	tmp[BUFSIZ];
	printf("%s <= %s\n", relpath(tmp, (char *)0, s), s);
}

_MAIN
{
	static	char	*tbl[] = {
		"RCS"
		,"."
		,".."
		,"../.."
		,"../bin"
		,"../../bin"
		,"../../bin/RCS"
#ifdef	apollo
		,"$(HOME)"
		,"$(HOME)/src"
#endif
		};
	auto	char	tmp[BUFSIZ];
	register int	j;

	if (argc > 1) {
		for (j = 1; j < argc; j++)
			do_test(argv[j]);
	} else {
		for (j = 0; j < sizeof(tbl)/sizeof(tbl[0]); j++) {
			do_test(tbl[j]);
			abspath(strcpy(tmp, tbl[j]));
			do_test(tmp);
		}
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
