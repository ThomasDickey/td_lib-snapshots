#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: vcs_file.c,v 12.2 1993/11/27 17:23:32 dickey Exp $";
#endif

/*
 * Title:	vcs_file (VCS permission- and temporary-file)
 * Author:	T.E.Dickey
 * Created:	13 Mar 1989
 * Modified:
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

char *
vcs_file(
_ARX(char *,	path)		/* pathname to prepend, if nonnull */
_ARX(char *,	name)		/* where to write the name */
_AR1(int,	temp)		/* TRUE iff we provide temporary-name */
	)
_DCL(char *,	path)
_DCL(char *,	name)
_DCL(int,	temp)
{
	register char	*s = pathleaf(rcs_dir());

	if (!strcmp(s, ".")
	||  isSlash(*s))
		s = RCS_DIR;
	(void)strcpy(name,s);
	if (!temp)
		(void)strcat(name, RCS_SUFFIX);
	return ((path == 0) ? name : pathcat(name, path, name));
}

#ifdef	TEST
_MAIN
{
	char	bfr[BUFSIZ];

	PRINTF("vcs_file='%s'\n", vcs_file("./", bfr,0));
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
