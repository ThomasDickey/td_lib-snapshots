#ifndef	lint
static	char	Id[] = "$Id: mem2file.c,v 11.0 1991/10/04 07:35:16 ste_cm Rel $";
#endif

/*
 * Title:	mem2file.c (memory-to-file I/O)
 * Author:	T.E.Dickey
 * Created:	11 May 1989
 * Modified:
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	writes a string to a given file.  The mode of the file is
 *		preserved if it already exists.
 *
 * Returns:	the number of bytes successfully written, or -1 if an error
 *		is found.
 */

#define	STR_PTYPES
#include	"ptypes.h"

mem2file(
_ARX(char *,	blob)
_ARX(char *,	name)
_AR1(char *,	mode)
	)
_DCL(char *,	blob)
_DCL(char *,	name)
_DCL(char *,	mode)
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
