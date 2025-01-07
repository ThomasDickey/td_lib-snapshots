/*
 * Title:	pathcmp.c (pathname compare)
 * Author:	T.E.Dickey
 * Created:	16 Mar 1989
 * Modified:
 *		26 Dec 2014, coverity warnings
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

MODULE_ID("$Id: pathcmp.c,v 12.9 2025/01/07 00:16:58 tom Exp $")

#define	CUT(p,s)	if ((s = strchr(p, PATH_SLASH)) != NULL) *s = EOS

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
pathcmp(const char *p1, const char *p2)
{
    char *s1, *s2;
    int code;
    char tmp1[MAXPATHLEN], tmp2[MAXPATHLEN];

    if (strlen(p1) < sizeof(tmp1) && strlen(p2) < sizeof(tmp2)) {
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
	    if ((s1 != NULL) || (s2 != NULL)) {
		if (s2 == NULL)
		    return (GT);
		if (s1 == NULL)
		    return (LT);
		p1 = s1 + 1;
		p2 = s2 + 1;
	    } else {
		break;
	    }
	}
	code = (*p1 - *p2);
    } else {
	code = EQ;
    }
    return code;
}

#ifdef	TEST
/*
 * Test by comparing all combinations of the specified pathnames
 */
#define	LOOP(i)	for (i = 1; i < argc; i++)

static void
do_test(int argc, const char **argv)
{
    int j, k;
    int len = 0;

    LOOP(j) {
	if ((k = (int) strlen(argv[j])) > len)
	    len = k;
    }

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
    if (argc > 1) {
	do_test(argc, (const char **) argv);
    } else {
	static const char *vec[] =
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
