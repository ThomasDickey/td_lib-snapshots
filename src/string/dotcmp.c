/*
 * Title:	dotcmp.c (compare dot-separated strings)
 * Author:	T.E.Dickey
 * Created:	19 May 1988
 * Modified:
 *		24 May 2010, change params to const.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *
 * Function:	Compare two strings, treating '.' as a column-gap; columns
 *		are right-justified.  This is useful for comparing paragraph
 *		numbers, etc.
 *
 * patch:	This does not handle strange cases with leading zeros, e.g.,
 *
 *			1.1, 1.05, 1.15
 */

#include "ptypes.h"

MODULE_ID("$Id: dotcmp.c,v 12.6 2010/07/05 16:02:09 tom Exp $")

#define	DOT	'.'
#define	LAST(s,l)	for (l = 0; (s[l] != DOT) && (s[l] != EOS); l++)

int
dotcmp(const char *s1, const char *s2)
{
    int cmp1, cmp2;

    while ((*s1 != EOS) || (*s2 != EOS)) {
	LAST(s1, cmp1);
	LAST(s2, cmp2);
	if (cmp1 == cmp2) {	/* same lengths, comparable */
	    while (cmp1-- > 0) {
		if ((cmp2 = (*s1++ - *s2++)) != EOS)
		    return (cmp2);
	    }
	    if ((*s1 != EOS) ^ (*s2 != EOS))
		return (*s1 - *s2);
	    if (*s1 == DOT)
		s1++;
	    if (*s2 == DOT)
		s2++;
	} else
	    return (cmp1 - cmp2);
    }
    return (0);
}

#ifdef	TEST
#include "td_qsort.h"

static
QSORT_FUNC(compare)
{
    QSORT_CAST(q1, p1);
    QSORT_CAST(q2, p2);
    return (dotcmp(*p1, *p2));
}

_MAIN
{
    int j;

    if (argc > 1) {
	qsort((char *) &argv[1], (size_t) (argc - 1), sizeof(argv[0]), compare);
	for (j = 1; j < argc; j++)
	    printf("%3d: %s\n", j, argv[j]);
    }
    (void) exit(SUCCESS);
    /*NOTREACHED */
}
#endif
