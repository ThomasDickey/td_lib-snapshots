#ifndef	lint
static	char	sccs_id[] = "@(#)copyback.c	1.2 88/08/11 07:41:16";
#endif	lint

/*
 * Title:	copyback.c (copy file back from temp-file)
 * Author:	T.E.Dickey
 * Created:	25 May 1988, from 'checkin'.
 * Modified:
 *
 * Function:	This is invoked to end a post-processing operation on a given
 *		file.  A given number of records has been written to a temporary
 *		file, which is here copied back to the original file.
 */

#include	"ptypes.h"
#include	<signal.h>

copyback (fpT, name, mode, lines)
FILE	*fpT;			/* temporary-file pointer */
char	*name;
int	mode;			/* original mode of file */
int	lines;			/* number of lines to copy (truncate) */
{
	FILE	*fpS;
	char	bfr[BUFSIZ];
	int	(*sv_int)();
	int	(*sv_quit)();

	if (chmod(name, 0644) < 0) {
		perror(name);
		return (FALSE);
	}
	sv_int  = signal (SIGINT, SIG_IGN);
	sv_quit = signal (SIGQUIT, SIG_IGN);
	if (fpS = fopen (name, "w")) {
		(void) rewind (fpT);
		while (lines-- > 0) {
			(void) fgets (bfr, sizeof(bfr), fpT);
			(void) fputs (bfr, fpS);
		}
		(void) fclose (fpS);
		(void) chmod (name, mode);
		(void) signal (SIGINT,  sv_int);
		(void) signal (SIGQUIT, sv_quit);
		return (TRUE);
	}
	return (FALSE);
}
