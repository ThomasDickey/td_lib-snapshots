#ifndef	lint
static	char	Id[] = "$Id: pathcmp.c,v 9.0 1991/05/15 10:04:23 ste_cm Rel $";
#endif

/*
 * Title:	pathcmp.c (pathname compare)
 * Author:	T.E.Dickey
 * Created:	16 Mar 1989
 * $Log: pathcmp.c,v $
 * Revision 9.0  1991/05/15 10:04:23  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  10:04:23  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  89/03/16  08:46:28  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  89/03/16  08:46:28  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/03/16  08:46:28  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/03/16  08:46:28  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  89/03/16  08:46:28  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  89/03/16  08:46:28  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  89/03/16  08:46:28  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  89/03/16  08:46:28  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Compare two unix-style pathnames so that they can be sorted.
 *		We must do piecewise comparison to avoid confusion with the
 *		characters which are lexically lower than '/' (e.g., '.').
 */

#define		STR_PTYPES
#include	"ptypes.h"

#ifndef	SLASH
#define	SLASH	'/'	/* divider between levels */
#endif

#define	CUT(p,s)	if ((s = strchr(p, SLASH)) != 0) *s = EOS

			/* TRACE makes the code more verbose */
#ifdef	DEBUG
#define	TRACE(s)	PRINTF s;
#else
#define	TRACE(s)
#endif

#define	LT	-1
#define	EQ	0
#define	GT	1

pathcmp(p1, p2)
char	*p1, *p2;
{
	register char	*s1, *s2;
	auto	int	code;
	auto	char	tmp1[BUFSIZ],
			tmp2[BUFSIZ];

	/* put the names into canonical form if they are not */
	abspath(p1 = strcpy(tmp1, p1));
	abspath(p2 = strcpy(tmp2, p2));

	TRACE(("\n\t(%s %s)", p1, p2))
	while (*p1 == SLASH && *p2 == SLASH)
		p1++, p2++;		/* faster than strcmp... */
	while (*p1 && *p2) {
		CUT(p1,s1);
		CUT(p2,s2);
		TRACE((" [%s %s]", p1,p2))
		if (code = strcmp(p1,p2))
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
	return (EQ);
}

#ifdef	TEST
/*
 * Test by comparing all combinations of the specified pathnames
 */
#define	LOOP(i)	for (i = 1; i < argc; i++)
do_test(argc, argv)
char	*argv[];
{
	register int	j, k;
	auto	 int	len	= 0;

	LOOP(j)
		if ((k = strlen(argv[j])) > len)
			len = k;

	LOOP(j) {
		PRINTF("%-*s ", len, argv[j]);
		LOOP(k) {
			int	code = pathcmp(argv[j], argv[k]);
			PRINTF(" %s",
				(code < 0) ? "<" : ((code > 0) ? ">" : "="));
		}
		PRINTF("\n");
	}
}

main(argc, argv)
char	*argv[];
{
	if (argc > 1)
		do_test(argc, argv);
	else {
		static	char	*vec[] = {
			 "?"
			,"/x"
			,"/x/y"
			,"/x/y/z"
			,"/x.y"
			,"/x.y/z"
#ifdef	apollo
			,"//a"
			,"//x"
#endif
			};
		do_test(sizeof(vec)/sizeof(vec[0]), vec);
	}
	(void)exit(SUCCESS);
}
#endif
