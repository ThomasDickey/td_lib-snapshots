/*
 * Title:	pathleaf.c (obtain leaf from path)
 * Author:	T.E.Dickey
 * Created:	18 Aug 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		04 Oct 1989, lint (apollo SR10.1)
 *		
 * Function:	Returns the lowest leaf-name on the given path by looking for
 *		the last path-delimiter.
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

MODULE_ID("$Id: pathleaf.c,v 12.5 2004/03/07 22:03:45 tom Exp $")

char *
pathleaf(char *path)
{
    char *s;
    static char buffer[BUFSIZ];

    path = strcpy(buffer, path);
    while ((s = fleaf_delim(path)) != NULL) {
#ifdef	apollo
	if (!strcmp(path, "//"))
	    break;
#endif
#ifndef	vms
	if (s[1] == EOS) {	/* trailing delimiter ? */
	    if (path == s)
		break;
	    *s = EOS;		/* trim it */
	} else
#endif
	    return (++s);
    }
    return (path);
}

#ifdef	TEST
_MAIN
{
    int j;
    for (j = 1; j < argc; j++)
	printf("%d:\t\"%s\" => \"%s\"\n",
	       j,
	       argv[j],
	       pathleaf(argv[j]));
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif /* TEST */
