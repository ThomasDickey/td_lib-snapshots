#ifndef	lint
static	char	Id[] = "$Id: vercmp.c,v 9.0 1991/05/15 09:58:23 ste_cm Rel $";
#endif

/*
 * Title:	vercmp.c (compare dot-separated version strings)
 * Author:	T.E.Dickey
 * Created:	14 Dec 1988 (from 'dotcmp()')
 * $Log: vercmp.c,v $
 * Revision 9.0  1991/05/15 09:58:23  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:58:23  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  88/12/14  11:16:57  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  88/12/14  11:16:57  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/12/14  11:16:57  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/12/14  11:16:57  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/12/14  11:16:57  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/12/14  11:16:57  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/12/14  11:16:57  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  88/12/14  11:16:57  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Compare two digit-strings, with '.' as a column-gap.
 *		Unlike 'dotcmp()', this has a flag 'wild' which governs the
 *		handling of strings which are of unequal length.  For RCS,
 *		we want to have
 *			2 >= 2.9 > 2.0
 *		To do this, we treat unspecified (sub)versions as a wildcard.
 *		The RCS tool must then simply test for a match.
 */

#define	EOS	'\0'
#define	DOT	'.'

#define	FIRST(s)	while (*s == '0' && s[1] != DOT && s[1] != EOS) s++
#define	LAST(s,l)	for (l = 0; (s[l] != DOT) && (s[l] != EOS); l++)

#ifdef	TEST
#define	TRACE(s)	printf s;
#else
#define	TRACE(s)
#endif

vercmp(s1, s2, wild)
char	*s1, *s2;
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
				if (cmp2 = (*s1++ - *s2++))
					return (cmp2);
			}
			if ((*s1 != EOS) ^ (*s2 != EOS))
				return wild ? 0 : (*s1 - *s2);
			if (*s1 == DOT)	s1++;
			if (*s2 == DOT)	s2++;
		} else if (wild && (cmp1 == 0) || (cmp2 == 0)) {
			return (0);
		} else {
			return (cmp1-cmp2);
		}
		TRACE(("\t\tnext '%s' vs '%s'\n", s1, s2))
	}
	return (0);
}

#ifdef	TEST
static	int	wild;

compare(p1, p2)
char	**p1, **p2;
{
	return (vercmp(*p1, *p2, wild));
}

#define	EQL(c)		((c > 0) ? ">" : ((c < 0) ? "<" : "="))
#define	DO_TEST(a,b)	j = vercmp("a","b", wild);\
			printf("%s\t%s %s \t(%d)\n", "a", EQL(j), "b", j)

do_test(argc, argv)
char	*argv[];
{
	register int	j;

	if (argc > 1) {
		qsort((char *)&argv[1], argc-1, sizeof(argv[0]), compare);
		for (j = 1; j < argc; j++)
			printf("%3d: %s\n", j, argv[j]);
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

main(argc, argv)
char	*argv[];
{
	if (argc > 1 && !strcmp(argv[1], "-w")) {
		argc--;
		argv++;
		wild++;
	}
	do_test(argc, argv);
	(void)exit(0);
	/*NOTREACHED*/
}
#endif
