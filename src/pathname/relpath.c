/*
 * Title:	relpath.c (convert path to relative-form)
 * Author:	T.E.Dickey
 * Created:	07 Sep 1989
 * Modified:
 *		27 Dec 2014, coverity warnings.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
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

MODULE_ID("$Id: relpath.c,v 12.10 2014/12/27 22:46:31 tom Exp $")

char *
relpath(char *dst, const char *cwd_param, const char *src)
{
    char current[MAXPATHLEN];
    char tmp[MAXPATHLEN];
    char pre[MAXPATHLEN];
    char *cwd = 0;
    size_t j;

    if (strlen(src) < sizeof(tmp)) {
	/* dst may be the same as src; copy it */
	src = strcpy(tmp, src);
	if (cwd_param == 0 || !*cwd_param) {
	    /* if cwd not given, get the actual path */
	    cwd = getwd(current);
	} else if (strlen(cwd_param) < sizeof(current)) {
	    cwd = strcpy(current, cwd_param);
	}
    }

    if (cwd != 0) {
#ifdef	apollo
	char *p;

#define	L_PAREN	'('
#define	R_PAREN	')'
	/*
	 * Substitute environment-variables
	 */
	if ((p = strchr(src, '$'))
	    && (p[1] == L_PAREN)) {
	    char tmp2[MAXPATHLEN];
	    char *s, *d;
	    int cc;

	    for (d = tmp2, s = src; *s; s++) {
		if (s[0] != '$')
		    *d++ = *s;
		else if (s[1] != L_PAREN)
		    *d++ = *s;
		else {
		    s += 2;
		    for (p = s; *p && *p != R_PAREN; p++) ;
		    cc = *p;
		    *p = EOS;
		    if (s = getenv(s)) {
			(void) strcpy(d, s);
			d += strlen(d);
		    }
		    *p = cc;
		    s = p;
		}
	    }
	    *d = EOS;
	    (void) strcpy(src, tmp2);
	}

	/*
	 * Factor-out common node-name
	 */
	if ((strlen(src) > 2)
	    && (strlen(cwd) > 2)
	    && !strncmp(cwd, "//", 2)) {
	    size_t nodelen;
	    for (nodelen = 2; cwd[nodelen]; nodelen++)
		if (isSlash(cwd[nodelen]))
		    break;
	    if (!strncmp(cwd, src, nodelen)) {
		cwd += nodelen;
		src += nodelen;
	    }
	}
#endif
	(void) strcpy(pre, ".");
	for (;;) {
	    if (!strcmp(cwd, src)) {
		(void) strcpy(dst, pre);
		break;
	    }

	    if (((j = strlen(cwd)) < strlen(src))
		&& !strncmp(cwd, src, j)
		&& isSlash(src[j])) {
		(void) pathcat(dst, pre, src + j + 1);
		break;
	    }

	    if (fleaf_delim(src) == 0) {
		if (dotname(src))
		    (void) strcpy(dst, src);
		else
		    (void) pathcat(dst, pre, src);
		break;
	    }

	    /*
	     * Trim off a leaf from current-directory, add a level
	     * of ".." to prefix:
	     */
	    (void) strcat(pre, pre[1] ? "/.." : ".");
	    if (j > 0 && !isSlash(cwd[j - 1])) {
		j--;
		while (!isSlash(cwd[j]))
		    cwd[j--] = EOS;
		if (j > 0 && !isSlash(cwd[j - 1]))
		    cwd[j] = EOS;
	    } else {
		(void) strcpy(dst, src);
		break;
	    }
	}
    } else {
	(void) strcpy(dst, src);
    }
    if (*dst == '.' && isSlash(dst[1])) {
	char *s, *d;
	for (s = dst + 2, d = dst; (*d++ = *s++) != EOS;) ;
    }
    return dst;
}

#ifdef	TEST
static void
do_test(const char *s)
{
    char tmp[BUFSIZ];
    PRINTF(" %s <= %s\n", relpath(tmp, (char *) 0, s), s);
}

_MAIN
{
    static const char *tbl[] =
    {
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
    char tmp[MAXPATHLEN];
    int j;

    if (argc > 1) {
	for (j = 1; j < argc; j++)
	    do_test(argv[j]);
    } else {
	for (j = 0; j < (int) SIZEOF(tbl); j++) {
	    PRINTF("case %2d) ", j);
	    do_test(tbl[j]);
	    abspath(strcpy(tmp, tbl[j]));
	    PRINTF("         ");
	    do_test(tmp);
	}
    }
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif /* TEST */
