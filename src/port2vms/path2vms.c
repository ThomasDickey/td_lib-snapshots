#ifndef	lint
static	char	Id[] = "$Id: path2vms.c,v 7.0 1991/05/20 17:17:03 ste_cm Rel $";
#endif

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
