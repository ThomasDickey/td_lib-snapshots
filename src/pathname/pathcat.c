#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/pathname/RCS/pathcat.c,v 4.0 1988/09/13 11:46:17 ste_cm Rel $";
#endif	lint

/*
 * Title:	pathcat.c (path concatenation)
 * Author:	T.E.Dickey
 * Created:	12 Sep 1988
 * $Log: pathcat.c,v $
 * Revision 4.0  1988/09/13 11:46:17  ste_cm
 * BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *
 *		Revision 3.0  88/09/13  11:46:17  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/09/13  11:46:17  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.3  88/09/13  11:46:17  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Forms a "path/filename" string, given the two parts, and
 *		ensures that we treat existing "/" marks intelligently.
 *
 *		Assumes that 'fname' and 'dst' are distinct buffers.  The
 *		'dst' and 'dname' buffers may be the same, though.
 */

#include	<stdio.h>
extern	char	*strcat();
extern	char	*strrchr();
extern	char	*strcpy();

char *
pathcat(dst, dname, fname)
char	*dst, *dname, *fname;
{
	auto	char	tmp[BUFSIZ],
			*s;

	if (*fname == '/')
		return (strcpy(dst, fname));
	(void)strcpy(tmp, dname);
	if ((s = strrchr(tmp, '/')) && (!strcmp(s, "/")))
		*s = '\0';		/* trim excess '/' */
	return (strcpy(dst, (strcat(strcat(tmp, "/"), fname))));
}
