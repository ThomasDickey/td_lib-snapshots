#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: pathleaf.c,v 12.2 1993/10/29 17:35:25 dickey Exp $";
#endif

/*
 * Title:	pathleaf.c (obtain leaf from path)
 * Author:	T.E.Dickey
 * Created:	18 Aug 1988
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		04 Oct 1989, lint (apollo SR10.1)
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
pathleaf (
_AR1(char *,	path))
_DCL(char *,	path)
{
	register char  *s;
	static	char	buffer[BUFSIZ];

	path	= strcpy(buffer, path);
	while ((s = strrchr(path, '/')) != NULL) {
		if (s[1] == EOS) {	/* trailing '/' ? */
			if (!strcmp(path, "/"))		break;
#ifdef	apollo
			if (!strcmp(path, "//"))	break;
#endif
			*s = EOS;	/* trim it */
		} else
			return (++s);
	}
	return (path);
}

#ifdef	TEST
_MAIN
{
	register int j;
	for (j = 1; j < argc; j++)
		printf("%d:\t\"%s\" => \"%s\"\n",
			j,
			argv[j],
			pathleaf(argv[j]));
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
