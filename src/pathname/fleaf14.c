/*
 * Title:	fleaf14.c
 * Author:	T.E.Dickey
 * Created:	14 Oct 1995
 *
 * Function:	truncates the leaf of the given pathname to 14 characters,
 *		for use with almost-obsolete file systems (and applications
 *		that ought to be obsolete).
 */

#define STR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: fleaf14.c,v 12.2 1995/10/14 15:03:51 tom Exp $")

char *	fleaf14(
	_AR1(char *, path))
	_DCL(char *, path)
{
	char	*leaf = fleaf(path);
	if (leaf == 0)
		leaf = path;
	if (strlen(leaf) > 14)
		leaf[14] = EOS;
	return path;
}
