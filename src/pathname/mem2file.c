#ifndef	lint
static	char	Id[] = "$Id: mem2file.c,v 9.0 1991/05/15 09:28:06 ste_cm Rel $";
#endif

/*
 * Title:	mem2file.c (memory-to-file I/O)
 * Author:	T.E.Dickey
 * Created:	11 May 1989
 *
 * Function:	writes a string to a given file.  The mode of the file is
 *		preserved if it already exists.
 *
 * Returns:	the number of bytes successfully written, or -1 if an error
 *		is found.
 */

#define	STR_PTYPES
#include	"ptypes.h"

mem2file(blob, name, mode)
char	*blob,*name,*mode;
{
	auto	int		len;
	auto	int		save;
	auto	struct stat	sb;
	auto	FILE		*fp;

	if (mode[0] != 'a' && mode[0] != 'w')
		return (-1);

	if (stat(name, &sb) >= 0) {
		if ((sb.st_mode & S_IFMT) != S_IFREG)
			return (-1);
		if ( ( (save = (sb.st_mode & 0777)) & 0222) == 0)
			if (chmod(name, save | 0600) < 0)
				return (-1);
	} else
		save = -1;

	if (fp = fopen(name, mode)) {

		len = fwrite(blob, sizeof(char), (int)strlen(blob), fp);
		(void)fclose(fp);

		if ((save >= 0)
		&&  (chmod(name,save) < 0))
			return (-1);
	}
	return (len);
}
