#ifndef	lint
static	char	Id[] = "$Id: relpath.c,v 5.1 1989/11/30 10:44:52 dickey Exp $";
#endif	lint

/*
 * Title:	relpath.c (convert path to relative-form)
 * Author:	T.E.Dickey
 * Created:	07 Sep 1989
 *
 * Function:	Converts a pathname from (presumably absolute form) to relative
 *		form.
 */

#define	STR_PTYPES
#include	"ptypes.h"

char	*
relpath(dst, cwd, src)
char	*dst, *cwd, *src;
{
	extern	char	*pathcat();
	auto	char	current[BUFSIZ];
	auto	char	tmp[BUFSIZ];
	auto	char	pre[BUFSIZ];
	auto	int	j;

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
do_test(s)
char	*s;
{
	auto	char	tmp[BUFSIZ];
	printf("%s <= %s\n", relpath(tmp, (char *)0, s), s);
}

main(argc, argv)
char	*argv[];
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
#endif	apollo
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
#endif	TEST
