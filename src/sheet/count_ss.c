#ifndef	lint
static	char	*Id = "$Id: count_ss.c,v 11.0 1992/02/03 12:03:23 ste_cm Rel $";
#endif

/*
 * Title:	count_ss.c
 * Author:	T.E.Dickey
 * Created:	03 Feb 1992
 *
 * Function:	counts rows/columns in a spreadsheet-table (in memory via
 *		'file2argv()').
 */

#include "spreadsheet.h"


int
count_ss_cols(
_AR1(char **,	vec))
_DCL(char **,	vec)
{
	auto	char	temp[BUFSIZ];
	auto	int	count	= 0;

	if (vec && *vec)
		while (get_field_of(*vec, count, temp, (char *)0))
			count++;
	return count;
}

int
count_ss_rows(
_AR1(char **,	vec))
_DCL(char **,	vec)
{
	return veclen(vec);
}
