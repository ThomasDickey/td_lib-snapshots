#ifndef	lint
static	char	Id[] = "$Id: sameleaf.c,v 9.0 1991/05/15 09:48:56 ste_cm Rel $";
#endif

/*
 * Title:	sameleaf.c (compare to determine if same-leaf of paths)
 * Author:	T.E.Dickey
 * Created:	19 Sep 1988
 * $Log: sameleaf.c,v $
 * Revision 9.0  1991/05/15 09:48:56  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:48:56  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  89/10/04  12:15:44  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  89/10/04  12:15:44  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/10/04  12:15:44  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/10/04  12:15:44  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.1  89/10/04  12:15:44  dickey
 *		lint (apollo SR10.1)
 *		
 *		Revision 4.0  88/09/19  07:40:26  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/09/19  07:40:26  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/09/19  07:40:26  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  88/09/19  07:40:26  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Given a test-pathname and a (possibly relative pathname) for
 *		a comparison-leafname, determine if they could be the same.
 *		This is used to compare RCS/SCCS directory-names to a pathname
 *		to see if they match.  For example, if
 *
 *			leaf == "SCCS"
 *
 *		then we match
 *
 *			"path/SCCS"
 *			"SCCS"
 *
 *		If
 *
 *			leaf == "../SCCS"
 *
 *		then we will strip off the "../" and match anything ending with
 *		"SCCS".
 */

#define	STR_PTYPES
#include	"ptypes.h"

sameleaf(path,leaf)
char	*path, *leaf;
{
	auto	 int	adjust;
	auto	 char	tmp[BUFSIZ];
	register char	*s;

	(void)strcpy(tmp, path);
	while (!strncmp(leaf, "../", 3))
		leaf += 3;
	while (s = strrchr(tmp, '/')) {	/* find real leaf-name */
		if (*(++s))
			break;
		*(--s) = '\0';		/* ...trimming off trailing '/' */
	}
	if (s == 0)
		s = tmp;

	/*
	 * Even after trimming, 'leaf' may contain a '/'.  If so, we must
	 * readjust the pointer to the path which we compare:
	 */
	if ((adjust = strlen(s) - strlen(leaf)) < 0) {
		if (s + adjust == tmp)
			s = tmp;
		else if (((s + adjust) > tmp) && (s[adjust-1] == '/'))
			s += adjust;
	}
#ifdef	TEST
	printf("\tcompare \"%s\" \"%s\"\n", s, leaf);
#endif
	return (!strcmp(s, leaf));
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
	register int	j;
	for (j = 1; j < argc; j++)
		printf("%d:\t%s %s => %d\n",
			j, argv[1], argv[j],
			sameleaf(argv[1], argv[j]));
}
#endif
