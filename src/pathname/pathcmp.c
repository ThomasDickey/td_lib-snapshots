/*
 * Title:	pathcmp.c (pathname compare)
 * Author:	T.E.Dickey
 * Created:	16 Mar 1989
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		12 May 1992, assume arguments are absolute pathnames (eliminate
 *			     time-consuming call to 'abspath()').
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *
 * Function:	Compare two unix-style pathnames so that they can be sorted.
 *		We must do piecewise comparison to avoid confusion with the
 *		characters which are lexically lower than '/' (e.g., '.').
 */

#define		STR_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: pathcmp.c,v 12.5 2004/03/07 22:03:45 tom Exp $")

#define	CUT(p,s)	if ((s = strchr(p, PATH_SLASH)) != 0) *s = EOS

			/* TRACE makes the code more verbose */
#ifdef	DEBUG
#define	TRACE(s)	PRINTF s;
#else
#define	TRACE(s)
#endif

#define	LT	-1
#define	EQ	0
#define	GT	1

int
pathcmp(char *p1, char *p2)
{
    char *s1, *s2;
    int code;
    char tmp1[BUFSIZ], tmp2[BUFSIZ];

    /* put the names into canonical form if they are not */
    p1 = strcpy(tmp1, p1);
    p2 = strcpy(tmp2, p2);

    TRACE(("\n\t(%s %s)", p1, p2))
	while (*p1 == PATH_SLASH && *p2 == PATH_SLASH)
	p1++, p2++;		/* faster than strcmp... */
    while (*p1 && *p2) {
	CUT(p1, s1);
	CUT(p2, s2);
	TRACE((" [%s %s]", p1, p2))
	    if ((code = strcmp(p1, p2)) != 0)
	    return (code);
	if ((s1 != 0) || (s2 != 0)) {
	    if (s2 == 0)
		return (GT);
	    if (s1 == 0)
		return (LT);
	    p1 = s1 + 1;
	    p2 = s2 + 1;
	} else
	    break;
    }
    return (*p1 - *p2);
}

#ifdef	TEST
/*
 * Test by comparing all combinations of the specified pathnames
 */
#define	LOOP(i)	for (i = 1; i < argc; i++)

void
do_test(int argc, char **argv)
{
    int j, k;
    int len = 0;

    LOOP(j)
	if ((k = strlen(argv[j])) > len)
	len = k;

    LOOP(j) {
	PRINTF("%-*s ", len, argv[j]);
	LOOP(k) {
	    int code = pathcmp(argv[j], argv[k]);
	    PRINTF(" %s",
		   (code < 0) ? "<" : ((code > 0) ? ">" : "="));
	}
	PRINTF("\n");
    }
}

_MAIN
{
    if (argc > 1)
	do_test(argc, argv);
    else {
	static char *vec[] =
	{
	    "?"
	    ,"/x"
	    ,"/x/y"
	    ,"/x/y/z"
	    ,"/x.y"
	    ,"/x.y/z"
	/*cf: apollo */
	    ,"//a"
	    ,"//x"
	};
	do_test(SIZEOF(vec), vec);
    }
    (void) exit(SUCCESS);
}
#endif
