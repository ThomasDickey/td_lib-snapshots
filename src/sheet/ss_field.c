#ifndef	lint
static	char	*Id = "$Id: ss_field.c,v 11.0 1992/02/04 09:19:09 ste_cm Rel $";
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
_AR1(char *,	dftval)
	)
_DCL(char **,	vec)
_DCL(int,	row)
_DCL(int,	col)
_DCL(char *,	buffer)
_DCL(char *,	dftval)
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
_AR1(char *,	buffer)
	)
_DCL(char **,	vec)
_DCL(int,	row)
_DCL(int,	col)
_DCL(char *,	buffer)
{
	return set_field_of(vec[row], col, buffer);
}
