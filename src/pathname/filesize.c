#ifndef	lint
static	char	*Id = "$Id: filesize.c,v 9.1 1991/09/13 08:03:39 dickey Exp $";
#endif

/*
 * Title:	filesize.c
 * Author:	T.E.Dickey
 * Created:	13 Sep 1991
 *
 * Function:	returns a file's size (in bytes), or -1 if none is available.
 */

#include "ptypes.h"
#include <errno.h>

off_t
filesize(name)
char	*name;
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
