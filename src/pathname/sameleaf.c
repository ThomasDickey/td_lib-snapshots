/*
 * Title:	sameleaf.c (compare to determine if same-leaf of paths)
 * Author:	T.E.Dickey
 * Created:	19 Sep 1988
 * Modified:
 *		27 Dec 2014, coverity warnings.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
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

MODULE_ID("$Id: sameleaf.c,v 12.13 2025/01/07 00:30:52 tom Exp $")

int
sameleaf(const char *path, const char *leaf)
{
    static char dotdot[] =
    {'.', '.', PATH_SLASH, EOS};

    int adjust;
    char tmp[MAXPATHLEN];
    char *s;
    int result = -1;

    if (strlen(path) < sizeof(tmp)) {
	(void) strcpy(tmp, path);
	while (!strncmp(leaf, dotdot, (size_t) 3))
	    leaf += 3;
	while ((s = fleaf(tmp)) != NULL) {	/* find real leaf-name */
	    if (*s != EOS)
		break;
	    *(--s) = EOS;	/* ...trimming off trailing delimiter */
	}
	if (s == NULL)
	    s = tmp;

	/*
	 * Even after trimming, 'leaf' may contain a delimiter.  If so, we must
	 * readjust the pointer to the path which we compare:
	 */
	if ((adjust = (int) strlen(s) - (int) strlen(leaf)) < 0) {
	    if (s + adjust == tmp)
		s = tmp;
	    else if (((s + adjust) > tmp) && (isSlash(s[adjust - 1])))
		s += adjust;
	}
#ifdef	TEST
	printf("\tcompare \"%s\" \"%s\"\n", s, leaf);
#endif
	result = (!strcmp(s, leaf));
    }
    return result;
}

#ifdef	TEST
_MAIN
{
    int j;
    for (j = 1; j < argc; j++)
	printf("%d:\t%s %s => %d\n",
	       j, argv[1], argv[j],
	       sameleaf(argv[1], argv[j]));
    exit(SUCCESS);
}
#endif /* TEST */
