#ifndef	lint
static	char	Id[] = "$Id: dotcmp.c,v 9.0 1991/05/15 10:01:35 ste_cm Rel $";
#endif

/*
 * Title:	dotcmp.c (compare dot-separated strings)
 * Author:	T.E.Dickey
 * Created:	19 May 1988
 * $Log: dotcmp.c,v $
 * Revision 9.0  1991/05/15 10:01:35  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  10:01:35  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  88/05/19  08:22:47  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  88/05/19  08:22:47  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/05/19  08:22:47  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/05/19  08:22:47  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/05/19  08:22:47  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/05/19  08:22:47  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/05/19  08:22:47  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  88/05/19  08:22:47  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Compare two strings, treating '.' as a column-gap; columns
 *		are right-justified.  This is useful for comparing paragraph
 *		numbers, etc.
 *
 * patch:	This does not handle strange cases with leading zeros, e.g.,
 *
 *			1.1, 1.05, 1.15
 */

#define	EOS	'\0'
#define	DOT	'.'

#define	LAST(s,l)	for (l = 0; (s[l] != DOT) && (s[l] != EOS); l++)

dotcmp(s1, s2)
char	*s1, *s2;
{
register int	cmp1, cmp2;

	while ((*s1 != EOS) || (*s2 != EOS)) {
		LAST(s1,cmp1);
		LAST(s2,cmp2);
		if (cmp1 == cmp2) {	/* same lengths, comparable */
			while (cmp1-- > 0) {
				if (cmp2 = (*s1++ - *s2++))
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
compare(p1, p2)
char	**p1, **p2;
{
	return (dotcmp(*p1, *p2));
}

main(argc, argv)
char	*argv[];
{
int	j;

	if (argc > 1) {
		qsort((char *)&argv[1], argc-1, sizeof(argv[0]), compare);
		for (j = 1; j < argc; j++)
			printf("%3d: %s\n", j, argv[j]);
	}
	(void)exit(0);
	/*NOTREACHED*/
}
#endif
