#ifndef	lint
static	char	Id[] = "$Id: pathleaf.c,v 8.0 1989/10/04 12:53:06 ste_cm Rel $";
#endif	lint

/*
 * Title:	pathleaf.c (obtain leaf from path)
 * Author:	T.E.Dickey
 * Created:	18 Aug 1988
 * $Log: pathleaf.c,v $
 * Revision 8.0  1989/10/04 12:53:06  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  89/10/04  12:53:06  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/10/04  12:53:06  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/10/04  12:53:06  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.1  89/10/04  12:53:06  dickey
 *		lint (apollo SR10.1)
 *		
 *		Revision 4.0  88/08/18  07:12:34  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/08/18  07:12:34  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/18  07:12:34  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  88/08/18  07:12:34  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Returns the lowest leaf-name on the given path by looking for
 *		the last '/'.
 *
 *		This always returns a pointer to a static buffer which is
 *		overwritten by successive calls.  Otherwise, it would have to
 *		modify its argument for the case in which a non-canonical path
 *		is given (i.e., with trailing non-significant slash).
 *
 *		If the name given is a root-level (i.e., "/"), then this is
 *		returned -- the returned string should never be empty.
 */

#define	STR_PTYPES
#include	"ptypes.h"

char *
pathleaf (path)
char	*path;
{
	register char  *s;
	static	char	buffer[BUFSIZ];

	path	= strcpy(buffer, path);
	while (s = strrchr(path, '/')) {
		if (s[1] == '\0') {	/* trailing '/' ? */
			if (!strcmp(path, "/"))		break;
#ifdef	apollo
			if (!strcmp(path, "//"))	break;
#endif	apollo
			*s = '\0';	/* trim it */
		} else
			return (++s);
	}
	return (path);
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
	register int j;
	for (j = 1; j < argc; j++)
		printf("%d:\t\"%s\" => \"%s\"\n",
			j,
			argv[j],
			pathleaf(argv[j]));
}
#endif	TEST
