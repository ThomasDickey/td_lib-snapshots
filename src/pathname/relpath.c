#ifndef	lint
static	char	Id[] = "$Id: relpath.c,v 11.1 1992/11/17 12:53:04 dickey Exp $";
#endif

/*
 * Title:	relpath.c (convert path to relative-form)
 * Author:	T.E.Dickey
 * Created:	07 Sep 1989
 * Modified:
 *		21 Jan 1992, If 'cwd[]' is empty, treat it as a null-pointer.
 *			     Also, force single return-point.
 *			     Strip special case of apollo node-name.
 *			     Expand apollo environment-variables.
 *			     Trim redundant leading "./".
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
	if (cwd == 0 || !*cwd)	/* if cwd not given, get the actual path */
		cwd = getwd(current);
	else
		cwd = strcpy(current, cwd);

	if (cwd != 0) {
#ifdef	apollo
		register char	*p;

#define	L_PAREN	'('
#define	R_PAREN	')'
		/*
		 * Substitute environment-variables
		 */
		if ((p = strchr(src, '$'))
		 && (p[1] == L_PAREN)) {
			char	tmp2[BUFSIZ];
			register char *s, *d;
			auto	int	cc;

			for (d = tmp2, s = src; *s; s++) {
				if (s[0] != '$')
					*d++ = *s;
				else if (s[1] != L_PAREN)
					*d++ = *s;
				else {
					s += 2;
					for (p = s; *p && *p != R_PAREN; p++);
					cc = *p;
					*p = EOS;
					if (s = getenv(s)) {
						(void)strcpy(d, s);
						d += strlen(d);
					}
					*p = cc;
					s = p;
				}
			}
			*d = EOS;
			(void)strcpy(src, tmp2);
		}

		/*
		 * Factor-out common node-name
		 */
		if ((strlen(src) > 2)
		 && (strlen(cwd) > 2)
		 && !strncmp(cwd, "//", 2)) {
			size_t	nodelen;
			for (nodelen = 2; cwd[nodelen]; nodelen++)
				if (cwd[nodelen] == '/')
					break;
			if (!strncmp(cwd, src, nodelen)) {
				cwd += nodelen;
				src += nodelen;
			}
		}
#endif
		(void)strcpy(pre, ".");
		for (;;) {
			if (	!strcmp(cwd,src)) {
				(void)strcpy(dst, pre);
				break;
			}

			if (	((j = strlen(cwd)) < strlen(src))
			&&	!strncmp(cwd,src,j)
			&&	src[j] == '/') {
				(void)pathcat(dst, pre, src+j+1);
				break;
			}

			if (strchr(src,'/') == 0) {
				if (dotname(src))
					(void)strcpy(dst, src);
				else
					(void)pathcat(dst, pre, src);
				break;
			}

			/*
			 * Trim off a leaf from current-directory, add a level
			 * of ".." to prefix:
			 */
			(void)strcat(pre, pre[1] ? "/.." : ".");
			if (j > 0 && cwd[--j] != '/') {
				while (cwd[j] != '/')
					cwd[j--] = EOS;
				if (j > 0 && cwd[j-1] != '/')
					cwd[j] = EOS;
			} else {
				(void)strcpy(dst, src);
				break;
			}
		}
	} else
		(void)strcpy(dst, src);
	if (*dst == '.' && dst[1] == '/') {
		register char	*s, *d;
		for (s = dst+2, d = dst; *d++ = *s++;);
	}
	return dst;
}

#ifdef	TEST
do_test(
_AR1(char *,	s))
_DCL(char *,	s)
{
	auto	char	tmp[BUFSIZ];
	PRINTF("%s <= %s\n", relpath(tmp, (char *)0, s), s);
}

_MAIN
{
	static	char	*tbl[] = {
		"RCS"
		,"."
		,".."
		,"../.."
		,"../bin"
		,"/bin"
		,"../../bin"
		,"../../bin/RCS"
#ifdef	apollo
		,"//dickey"
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
		for (j = 0; j < SIZEOF(tbl); j++) {
			do_test(tbl[j]);
			abspath(strcpy(tmp, tbl[j]));
			do_test(tmp);
		}
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
