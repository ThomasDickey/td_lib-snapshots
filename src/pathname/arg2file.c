#ifndef	lint
static	char	Id[] = "$Id: arg2file.c,v 12.1 1993/09/21 12:08:24 dickey Exp $";
#endif

/*
 * Title:	argv2file.c
 * Author:	T.E.Dickey
 * Created:	18 May 1989
 * Modified:
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	Write an argv-style text vector to a file, with the given mode.
 */

#define	STR_PTYPES
#include	"ptypes.h"

int	argv2file(
	_ARX(char **,	vec)
	_ARX(char *,	name)
	_AR1(char *,	mode)
		)
	_DCL(char **,	vec)
	_DCL(char *,	name)
	_DCL(char *,	mode)
{
	auto	FILE	*fp;
	auto	int	count;

	if ((fp = fopen(name, mode)) != 0) {
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
