#ifndef	lint
static	char	Id[] = "$Id: arg2file.c,v 8.0 1989/10/04 12:05:50 ste_cm Rel $";
#endif	lint

/*
 * Title:	argv2file.c
 * Author:	T.E.Dickey
 * Created:	18 May 1989
 *
 * Function:	Write an argv-style text vector to a file, with the given mode.
 */

#define	STR_PTYPES
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
