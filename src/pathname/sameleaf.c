#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/pathname/RCS/sameleaf.c,v 4.0 1988/09/19 07:40:26 ste_cm Rel $";
#endif	lint

/*
 * Title:	sameleaf.c (compare to determine if same-leaf of paths)
 * Author:	T.E.Dickey
 * Created:	19 Sep 1988
 * $Log: sameleaf.c,v $
 * Revision 4.0  1988/09/19 07:40:26  ste_cm
 * BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
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

#include	<stdio.h>
extern	char	*strcpy();
extern	char	*strrchr();

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
#endif	TEST
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
#endif	TEST
