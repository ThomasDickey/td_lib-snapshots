#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: vercmp.c,v 12.5 1994/05/30 23:36:25 tom Exp $";
#endif

/*
 * Title:	vercmp.c (compare dot-separated version strings)
 * Author:	T.E.Dickey
 * Created:	14 Dec 1988 (from 'dotcmp()')
 * Modified:
 *		29 Oct 1993, ifdef-ident; port to HP/UX.
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *
 * Function:	Compare two digit-strings, with '.' as a column-gap.
 *		Unlike 'dotcmp()', this has a flag 'wild' which governs the
 *		handling of strings which are of unequal length.  For RCS,
 *		we want to have
 *			2 >= 2.9 > 2.0
 *		To do this, we treat unspecified (sub)versions as a wildcard.
 *		The RCS tool must then simply test for a match.
 */

#include	"ptypes.h"

#define	DOT	'.'

#define	FIRST(s)	while (*s == '0' && s[1] != DOT && s[1] != EOS) s++
#define	LAST(s,l)	for (l = 0; (s[l] != DOT) && (s[l] != EOS); l++)

#ifdef	TEST
#define	TRACE(s)	PRINTF s;
#else
#define	TRACE(s)
#endif

int	vercmp(
	_ARX(char *,	s1)
	_ARX(char *,	s2)
	_AR1(int,	wild)
		)
	_DCL(char *,	s1)
	_DCL(char *,	s2)
	_DCL(int,	wild)
{
	register int	cmp1, cmp2;

	while ((*s1 != EOS) || (*s2 != EOS)) {
		FIRST(s1);
		FIRST(s2);
		LAST(s1,cmp1);
		LAST(s2,cmp2);
		TRACE(("\t\ttest '%s' vs '%s'\t('%.*s' vs '%.*s')\n",
			s1, s2, cmp1,s1, cmp2,s2))
		if (cmp1 == cmp2) {	/* same lengths, comparable */
			while (cmp1-- > 0) {
				if ((cmp2 = (*s1++ - *s2++)) != EOS)
					return (cmp2);
			}
			if ((*s1 != EOS) ^ (*s2 != EOS))
				return wild ? 0 : (*s1 - *s2);
			if (*s1 == DOT)	s1++;
			if (*s2 == DOT)	s2++;
		} else if (wild && ((cmp1 == 0) || (cmp2 == 0))) {
			return (0);
		} else {
			return (cmp1-cmp2);
		}
		TRACE(("\t\tnext '%s' vs '%s'\n", s1, s2))
	}
	return (0);
}

#ifdef	TEST
#include "td_qsort.h"

static	int	wild;

static
QSORT_FUNC(compare)
{
	QSORT_CAST(q1,p1)
	QSORT_CAST(q2,p2)
	return (vercmp(*p1, *p2, wild));
}

#define	EQL(c)		((c > 0) ? ">" : ((c < 0) ? "<" : "="))
#if	HAVE_NEW_TOKEN_QUOTE
#define	DO_TEST(a,b)	j = vercmp(#a, #b,  wild);\
			PRINTF("%s\t%s %s \t(%d)\n", #a,  EQL(j), #b,  j)
#else
#define	DO_TEST(a,b)	j = vercmp("a","b", wild);\
			PRINTF("%s\t%s %s \t(%d)\n", "a", EQL(j), "b", j)
#endif

static
void	do_test(
	_ARX(int,	argc)
	_AR1(char **,	argv)
		)
	_DCL(int,	argc)
	_DCL(char **,	argv)
{
	register int	j;

	if (argc > 1) {
		qsort((char *)&argv[1], argc-1, sizeof(argv[0]), compare);
		for (j = 1; j < argc; j++)
			PRINTF("%3d: %s\n", j, argv[j]);
	} else {
		DO_TEST(20,2.3);
		DO_TEST(2,20.3);
		DO_TEST(2,2.3);
		DO_TEST(2,2.0);
		DO_TEST(2,1.99);
		DO_TEST(2.,2.3);
		DO_TEST(2.0,2);
	}
}

_MAIN
{
	if (argc > 1 && !strcmp(argv[1], "-w")) {
		argc--;
		argv++;
		wild++;
	}
	do_test(argc, argv);
	(void)exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
