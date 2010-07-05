/*
 * Title:	is_subpath.c
 * Author:	T.E.Dickey
 * Created:	17 Feb 1992
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	Tests a pathname to see if it is a sub-path of the other.
 *
 *		This assumes both pathnames are in UNIX canonical form.
 *
 * Returns:	-1 if not, otherwise, the index in the test-path at which
 *		the common portion ends.
 */

#define	STR_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: is_spath.c,v 12.5 2010/07/04 15:28:07 tom Exp $")

int
is_subpath(char *name_ref, char *name_tst)
{
    size_t l_ref = strlen(name_ref);
    size_t l_tst = strlen(name_tst);

    if (l_ref < l_tst
	&& isSlash(name_tst[l_ref])
	&& !strncmp(name_ref, name_tst, l_ref))
	return (int) l_ref + 1;

    if (l_ref == l_tst
	&& !strcmp(name_ref, name_tst))
	return (int) l_ref;

    return -1;
}
