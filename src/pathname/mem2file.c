/*
 * Title:	mem2file.c (memory-to-file I/O)
 * Author:	T.E.Dickey
 * Created:	11 May 1989
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
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

MODULE_ID("$Id: mem2file.c,v 12.10 2025/01/07 00:30:52 tom Exp $")

int
mem2file(char *blob, char *name, char *mode)
{
    int len = -1;
    int save;
    Stat_t sb;
    FILE *fp;

    if (mode[0] != 'a' && mode[0] != 'w')
	return (-1);

    if (stat(name, &sb) >= 0) {
	if ((sb.st_mode & S_IFMT) != S_IFREG)
	    return (-1);
	if (((save = (int) (sb.st_mode & 0777)) & 0222) == 0)
	    if (chmod(name, (mode_t) (save | 0600)) < 0)
		return (-1);
    } else
	save = -1;

    if ((fp = fopen(name, mode)) != NULL) {

	len = (int) fwrite(blob, sizeof(char), strlen(blob), fp);
	(void) fclose(fp);

	if ((save >= 0)
	    && (chmod(name, (mode_t) save) < 0))
	    return (-1);
    }
    return (len);
}

/******************************************************************************/
#ifdef	TEST
_MAIN
{
    (void) argc;
    (void) argv;
    exit(EXIT_FAILURE);
    /*NOTREACHED */
}
#endif /* TEST */
