/*
 * Title:	samepath.c (compare pathname prefixes)
 * Author:	T.E.Dickey
 * Created:	13 Jul 1994
 * Modified:
 *		30 Apr 2003, correct split-out (check for slash or null).
 *		25 Apr 2003, split-out from sccs_dir.c
 *		
 * Compare two pathnames, returning the length of the matching portion,
 * limited to a pathname separator.  Note that 'strchr()' can use a null
 * character for the second argument.
 */

#define	STR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: samehead.c,v 12.3 2003/04/30 19:13:10 tom Exp $")

int	samehead(
	_ARX(char *,	path1)
	_AR1(char *,	path2)
		)
	_DCL(char *,	path1)
	_DCL(char *,	path2)
{
	int	match = 0;
	register int	n;

	for (n = 0; ; n++) {
		if ((isSlash(path1[n]) || path1[n] == EOS)
		 && (isSlash(path2[n]) || path2[n] == EOS))
			match = n;
		if (path1[n] == EOS || path2[n] == EOS)
			break;
		if (path1[n] != path2[n])
			break;
	}
	return match;
}
