#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: copyback.c,v 12.3 1994/07/16 15:23:38 tom Exp $";
#endif

/*
 * Title:	copyback.c (copy file back from temp-file)
 * Author:	T.E.Dickey
 * Created:	25 May 1988, from 'checkin'.
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, mods to compile under apollo sr10.3
 *		13 Sep 1988, use 'catchall()'.
 *
 * Function:	This is invoked to end a post-processing operation on a given
 *		file.  A given number of records has been written to a temporary
 *		file, which is here copied back to the original file.
 */

#define	SIG_PTYPES
#include	"ptypes.h"
#include	<signal.h>

int	copyback (
	_ARX(FILE *,	fpT)		/* temporary-file pointer */
	_ARX(char *,	name)
	_ARX(int,	mode)		/* original mode of file */
	_AR1(int,	lines)		/* number of lines to copy (truncate) */
		)
	_DCL(FILE *,	fpT)
	_DCL(char *,	name)
	_DCL(int,	mode)
	_DCL(int,	lines)
{
	FILE	*fpS;
	char	bfr[BUFSIZ];

	if (chmod(name, 0644) < 0) {
		perror(name);
		return (FALSE);
	}
	catchall(SIG_IGN);
	if ((fpS = fopen (name, "w")) != 0) {
		(void) rewind (fpT);
		while (lines-- > 0) {
			(void) fgets (bfr, sizeof(bfr), fpT);
			(void) fputs (bfr, fpS);
		}
		(void) fclose (fpS);
		(void) chmod (name, (mode_t)mode);
		catchall(SIG_DFL);
		return (TRUE);
	}
	return (FALSE);
}
