#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/port2vms/RCS/path2vms.c,v 4.0 1988/10/28 17:18:39 ste_cm Rel $";
#endif	lint

/*
 * Title:	path2vms.c
 * Author:	T.E.Dickey
 * Created:	30 Sep 1988
 * Modified:
 *		28 Oct 1988, return destination buffer
 *
 * Function:	Convert a unix-style pathname to a VMS-style pathname when we
 *		are certain that it is a directory-name, not a file.
 */

#define		STR_PTYPES
#include	"portunix.h"

char *
path2vms(dst, src)
char	*dst, *src;
{
	char	tmp[MAXPATHLEN];
	int	len = strlen(strcpy(tmp, src));
	if (len == 0 || tmp[len-1] != '/')
		(void)strcat(tmp, "/");
	return (name2vms(dst, tmp));
}
