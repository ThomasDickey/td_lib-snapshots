#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/pathname/RCS/arg2file.c,v 4.0 1989/05/18 11:19:49 ste_cm Rel $";
#endif	lint

/*
 * Title:	argv2file.c
 * Author:	T.E.Dickey
 * Created:	18 May 1989
 *
 * Function:	Write an argv-style text vector to a file, with the given mode.
 */

#include	"ptypes.h"

argv2file(vec, name, mode)
char	**vec;
char	*name;
char	*mode;
{
	auto	FILE	*fp;
	auto	int	count;

	if (fp = fopen(name, mode)) {
		count	= 0;
		while (*vec) {
			count += strlen(*vec);
			FPRINTF(fp, "%s\n", *vec++);
		}
		FCLOSE(fp);
	} else
		count = -1;

	return (count);
}
