#ifndef	lint
static	char	sccs_id[] = "@(#)rcsname.c	1.1 88/05/27 07:06:00";
#endif	lint

/*
 * Title:	rcsname.c (derive name of RCS file)
 * Author:	T.E.Dickey
 * Created:	27 May 1988
 * Modified:
 *
 * Function:	Given the name of a working file, derive the name for the
 *		corresponding RCS file.  This assumes that the RCS file is
 *		always in a subdirectory "./RCS".  Doing so exploits the
 *		assumptions of 'ci' and 'co' that permit giving a full pathname
 *		for 'name'.
 */

#include	"rcsdefs.h"

#include	<stdio.h>
#include	<ctype.h>
extern	char	*strcat();
extern	char	*strcpy();
extern	char	*strrchr();

char *
rcsname(name)
char	*name;
{
	char	*leaf	= strrchr(name, '/');
static	char	fname[BUFSIZ];

	return (strcat(
		strcat(
			strcat(
				strcpy(fname, RCS_DIR),
				"/"),
			leaf ? leaf + 1 : name),
		RCS_SUFFIX));
}
