#ifndef	lint
static	char	Id[] = "$Id: is_spath.c,v 12.0 1992/02/17 14:47:27 ste_cm Rel $";
#endif

/*
 * Title:	is_subpath.c
 * Author:	T.E.Dickey
 * Created:	17 Feb 1992
 * Modified:
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

int
is_subpath(
_ARX(char *,	name_ref)
_AR1(char *,	name_tst)
	)
_DCL(char *,	name_ref)
_DCL(char *,	name_tst)
{
	size_t	l_ref	= strlen(name_ref);
	size_t	l_tst	= strlen(name_tst);

	if ( l_ref < l_tst
	 &&  name_tst[l_ref] == '/'
	 &&  !strncmp(name_ref, name_tst, l_ref) )
		return l_ref + 1;

	if ( l_ref == l_tst
	 &&  !strcmp(name_ref, name_tst) )
		return l_ref;

	return -1;
}