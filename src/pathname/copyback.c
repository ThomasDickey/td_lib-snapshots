#ifndef	lint
static	char	Id[] = "$Id: copyback.c,v 9.0 1991/05/15 12:52:19 ste_cm Rel $";
#endif

/*
 * Title:	copyback.c (copy file back from temp-file)
 * Author:	T.E.Dickey
 * Created:	25 May 1988, from 'checkin'.
 * $Log: copyback.c,v $
 * Revision 9.0  1991/05/15 12:52:19  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  12:52:19  dickey
 *		mods to compile under apollo sr10.3
 *		
 *		Revision 8.0  88/09/13  09:03:19  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  88/09/13  09:03:19  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/09/13  09:03:19  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/09/13  09:03:19  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/09/13  09:03:19  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/09/13  09:03:19  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/09/13  09:03:19  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.4  88/09/13  09:03:19  dickey
 *		sccs2rcs keywords
 *		
 *		13 Sep 1988, use 'catchall()'.
 *
 * Function:	This is invoked to end a post-processing operation on a given
 *		file.  A given number of records has been written to a temporary
 *		file, which is here copied back to the original file.
 */

#define	SIG_PTYPES
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

	if (chmod(name, 0644) < 0) {
		perror(name);
		return (FALSE);
	}
	catchall(SIG_IGN);
	if (fpS = fopen (name, "w")) {
		(void) rewind (fpT);
		while (lines-- > 0) {
			(void) fgets (bfr, sizeof(bfr), fpT);
			(void) fputs (bfr, fpS);
		}
		(void) fclose (fpS);
		(void) chmod (name, mode);
		catchall(SIG_DFL);
		return (TRUE);
	}
	return (FALSE);
}
