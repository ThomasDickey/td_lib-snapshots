#ifndef	lint
static	char	Id[] = "$Id: vcs_file.c,v 9.0 1991/05/15 09:57:40 ste_cm Rel $";
#endif

/*
 * Title:	vcs_file (VCS permission- and temporary-file)
 * Author:	T.E.Dickey
 * Created:	13 Mar 1989
 * $Log: vcs_file.c,v $
 * Revision 9.0  1991/05/15 09:57:40  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:57:40  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  89/03/13  13:04:49  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  89/03/13  13:04:49  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/03/13  13:04:49  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/03/13  13:04:49  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  89/03/13  13:04:49  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  89/03/13  13:04:49  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  89/03/13  13:04:49  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  89/03/13  13:04:49  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	This encapsulates the name for the 'permit' utility's directory-
 *		access file, and the temporary file which is used to update it.
 *		Because the RCS-directory path may change (see 'rcs_dir()'), we
 *		provide a corresponding package for the 'permit' file, which
 *		depends upon it.
 */

#include	<string.h>
#include	"ptypes.h"
#include	"rcsdefs.h"
extern	char	*pathcat();
extern	char	*pathleaf();

char *
vcs_file(path, name,temp)
char	*path;		/* pathname to prepend, if nonnull */
char	*name;		/* where to write the name */
int	temp;		/* TRUE iff we provide temporary-name */
{
	register char	*s = pathleaf(rcs_dir());

	if (!strcmp(s, ".")
	||  (*s == '/'))
		s = RCS_DIR;
	(void)strcpy(name,s);
	if (!temp)
		(void)strcat(name, RCS_SUFFIX);
	return ((path == 0) ? name : pathcat(name, path, name));
}

#ifdef	TEST
main(argc,argv)
char	*argv[];
{
	char	bfr[BUFSIZ];

	printf("vcs_file='%s'\n", vcs_file("./", bfr,0));
}
#endif
