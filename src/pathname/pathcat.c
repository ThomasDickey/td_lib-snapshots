#ifndef	lint
static	char	sccs_id[] = "@(#)pathcat.c	1.1 88/09/12 15:11:43";
#endif	lint

/*
 * Title:	pathcat.c (path concatenation)
 * Author:	T.E.Dickey
 * Created:	12 Sep 1988
 * Modified:
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
	return (strcat(strcat(tmp, "/"), fname));
}
