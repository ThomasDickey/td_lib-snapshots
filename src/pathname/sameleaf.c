#ifndef	lint
static	char	Id[] = "$Id: sameleaf.c,v 12.1 1993/09/21 18:54:03 dickey Exp $";
#endif

/*
 * Title:	sameleaf.c (compare to determine if same-leaf of paths)
 * Author:	T.E.Dickey
 * Created:	19 Sep 1988
 * Modified:
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		04 Oct 1989, lint (apollo SR10.1)
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

int	sameleaf(
	_ARX(char *,	path)
	_AR1(char *,	leaf)
		)
	_DCL(char *,	path)
	_DCL(char *,	leaf)
{
	auto	 int	adjust;
	auto	 char	tmp[BUFSIZ];
	register char	*s;

	(void)strcpy(tmp, path);
	while (!strncmp(leaf, "../", 3))
		leaf += 3;
	while ((s = strrchr(tmp, '/')) != NULL) { /* find real leaf-name */
		if (*(++s))
			break;
		*(--s) = EOS;		/* ...trimming off trailing '/' */
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
_MAIN
{
	register int	j;
	for (j = 1; j < argc; j++)
		printf("%d:\t%s %s => %d\n",
			j, argv[1], argv[j],
			sameleaf(argv[1], argv[j]));
	exit(SUCCESS);
}
#endif
