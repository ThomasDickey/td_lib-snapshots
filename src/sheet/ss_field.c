#ifndef	lint
static	char	*Id = "$Id: ss_field.c,v 10.2 1992/02/03 14:35:09 dickey Exp $";
#endif

/*
 * Title:	ss_field.c
 * Author:	T.E.Dickey
 * Created:	03 Feb 1992
 *
 * Function:	insert/extract data from in-memory spreadsheet-table.
 */

#include "spreadsheet.h"

char *
get_ss_field(
_ARX(char **,	vec)
_ARX(int,	row)
_ARX(int,	col)
_ARX(char *,	buffer)
_AR1(char *,	dftval))
{
	if (vec)
		buffer = get_field_of(vec[row], col, buffer, dftval);
	else
		buffer = dftval;
	return (buffer);
}

char *
set_ss_field(
_ARX(char **,	vec)
_ARX(int,	row)
_ARX(int,	col)
_AR1(char *,	buffer))
{
	return set_field_of(vec[row], col, buffer);
}
