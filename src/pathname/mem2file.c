#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: mem2file.c,v 12.2 1993/10/29 17:35:25 dickey Exp $";
#endif

/*
 * Title:	mem2file.c (memory-to-file I/O)
 * Author:	T.E.Dickey
 * Created:	11 May 1989
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
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

int	mem2file(
	_ARX(char *,	blob)
	_ARX(char *,	name)
	_AR1(char *,	mode)
		)
	_DCL(char *,	blob)
	_DCL(char *,	name)
	_DCL(char *,	mode)
{
	auto	int	len	= -1;
	auto	int	save;
	auto	STAT	sb;
	auto	FILE	*fp;

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

	if ((fp = fopen(name, mode)) != 0) {

		len = fwrite(blob, sizeof(char), (LEN_FREAD)strlen(blob), fp);
		(void)fclose(fp);

		if ((save >= 0)
		&&  (chmod(name,save) < 0))
			return (-1);
	}
	return (len);
}
