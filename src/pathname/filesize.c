#ifndef	lint
static	char	*Id = "$Id: filesize.c,v 10.0 1991/10/04 10:02:45 ste_cm Rel $";
#endif

/*
 * Title:	filesize.c
 * Author:	T.E.Dickey
 * Created:	13 Sep 1991
 * Modified:
 *		04 Oct 1991, conversion to ANSI
 *
 * Function:	returns a file's size (in bytes), or -1 if none is available.
 */

#include "ptypes.h"
#include <errno.h>

off_t
filesize(
_AR1(char *,	name))
_DCL(char *,	name)
{
	auto	struct	stat	sb;

	if (stat(name, &sb) < 0)
		return -1;
	if ((sb.st_mode & S_IFMT) != S_IFREG) {
		errno = EISDIR;		/* want to say it's not a file! */
		return -1;
	}
	return (sb.st_size);
}
