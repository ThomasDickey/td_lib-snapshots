#ifndef	lint
static	char	Id[] = "$Id: dotcmp.c,v 12.1 1993/09/21 18:54:05 dickey Exp $";
#endif

/*
 * Title:	dotcmp.c (compare dot-separated strings)
 * Author:	T.E.Dickey
 * Created:	19 May 1988
 * Modified:
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

#define	DOT	'.'
#define	LAST(s,l)	for (l = 0; (s[l] != DOT) && (s[l] != EOS); l++)

int	dotcmp(
	_ARX(char *,	s1)
	_AR1(char *,	s2)
		)
	_DCL(char *,	s1)
	_DCL(char *,	s2)
{
	register int	cmp1, cmp2;

	while ((*s1 != EOS) || (*s2 != EOS)) {
		LAST(s1,cmp1);
		LAST(s2,cmp2);
		if (cmp1 == cmp2) {	/* same lengths, comparable */
			while (cmp1-- > 0) {
				if ((cmp2 = (*s1++ - *s2++)) != EOS)
					return (cmp2);
			}
			if ((*s1 != EOS) ^ (*s2 != EOS))
				return (*s1 - *s2);
			if (*s1 == DOT)	s1++;
			if (*s2 == DOT)	s2++;
		} else
			return (cmp1-cmp2);
	}
	return (0);
}

#ifdef	TEST
#include "td_qsort.h"

static
QSORT_FUNC(compare)
{
	QSORT_CAST(q1,p1)
	QSORT_CAST(q2,p2)
	return (dotcmp(*p1, *p2));
}

_MAIN
{
	register int	j;

	if (argc > 1) {
		qsort((char *)&argv[1], argc-1, sizeof(argv[0]), compare);
		for (j = 1; j < argc; j++)
			printf("%3d: %s\n", j, argv[j]);
	}
	(void)exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
