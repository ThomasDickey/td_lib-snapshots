/*
 * Title:	vcs_file (VCS permission- and temporary-file)
 * Author:	T.E.Dickey
 * Created:	13 Mar 1989
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *
 * Function:	This encapsulates the name for the 'permit' utility's directory-
 *		access file, and the temporary file which is used to update it.
 *		Because the RCS-directory path may change (see 'rcs_dir()'), we
 *		provide a corresponding package for the 'permit' file, which
 *		depends upon it.
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	"rcsdefs.h"

MODULE_ID("$Id: vcs_file.c,v 12.8 2025/01/06 23:59:56 tom Exp $")

char *
vcs_file(const char *path,	/* pathname to prepend, if nonnull */
	 char *name,		/* where to write the name */
	 int temp)		/* TRUE iff we provide temporary-name */
{
    const char *s = pathleaf(rcs_dir(NULL, NULL));

    if (!strcmp(s, ".")
	|| isSlash(*s))
	s = RCS_DIR;
    (void) strcpy(name, s);
    if (!temp)
	(void) strcat(name, RCS_SUFFIX);
    return ((path == NULL) ? name : pathcat(name, path, name));
}

#ifdef	TEST
_MAIN
{
    char bfr[BUFSIZ];

    (void) argc;
    (void) argv;

    PRINTF("vcs_file='%s'\n", vcs_file("./", bfr, 0));
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif
