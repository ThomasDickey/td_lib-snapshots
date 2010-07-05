/*
 * Title:	strbcmp.c (string-blank-compare)
 * Author:	T.E.Dickey
 * Created:	07 Apr 1989
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	compare two strings, treating sequences of embedded whitespace
 *		as a single blank, and ignoring trailing whitespace.  This is
 *		essentially the comparison performed by "diff -b".
 *
 * Returns:	the lexical difference between the two strings (0 if equal,
 *		positive if the first string would sort before the second,
 *		otherwise negative).
 */

#define STR_PTYPES
#define TIM_PTYPES
#include	<ptypes.h>
#include	<ctype.h>

MODULE_ID("$Id: strbcmp.c,v 12.6 2010/07/05 16:12:01 tom Exp $")

#define	SKIP(p)	while (isspace(UCH(*p)))	p++;

int
strbcmp(const char *a, const char *b)
{
    int cmp;

    while (*a && *b) {
	if (isspace(UCH(*a)) && isspace(UCH(*b))) {
	    SKIP(a);
	    SKIP(b);
	} else if ((cmp = (*a++ - *b++)) != EOS)
	    return (cmp);
    }
    SKIP(a);
    SKIP(b);
    return (*a - *b);
}

#ifdef	TEST
/*ARGSUSED*/
_MAIN
{
#ifdef	TEST2
    time_t now;
#endif
    static const char *tbl[] =
    {
	"ab",
	" ab",
	"a b",
	"ab ",
	" ab ",
	"a b "
    };
    int j, k;
#ifdef TEST2
    int cmp;
#endif

    (void) argc;
    (void) argv;

#define	LOOP(j)	for (j = 0; j < (int) SIZEOF(tbl); j++)
#define	CMP(f)	(f(tbl[j],tbl[k]) ? ((f(tbl[j],tbl[k]) > 0) ? ">" : "<") : "=")
    LOOP(j) {
	printf("\n");
	LOOP(k) {
	    printf("\"%s\"\t\"%s\"\t%s (blank) %s (normal)\n",
		   tbl[j], tbl[k],
		   CMP(strbcmp),
		   CMP(strcmp));
	}
    }
#ifdef	TEST2
    now = time(0);
    for (cmp = 0; cmp < 20000; cmp++)
	LOOP(j) {
	LOOP(k) {
	    (void) strbcmp(tbl[j], tbl[k]);
	}
	}
    printf("# %ld seconds\n", (long) (time(0) - now));
#endif
    (void) exit(SUCCESS);
    /*NOTREACHED */
}
#endif
