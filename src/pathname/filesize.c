#ifndef	lint
static	char	*Id = "$Id: filesize.c,v 12.0 1992/02/06 10:08:59 ste_cm Rel $";
#endif

/*
 * Title:	filesize.c
 * Author:	T.E.Dickey
 * Created:	13 Sep 1991
 * Modified:
 *		06 Feb 1992, use 'stat_file()'
 *		04 Oct 1991, conversion to ANSI
 *
 * Function:	returns a file's size (in bytes), or -1 if none is available.
 */

#include "ptypes.h"

off_t
filesize(
_AR1(char *,	name))
_DCL(char *,	name)
{
	auto	STAT	sb;

	return (stat_file(name, &sb) < 0) ? -1 : sb.st_size;
}
