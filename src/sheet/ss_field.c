#ifndef	lint
static	char	*Id = "$Id: ss_field.c,v 11.1 1992/08/03 08:26:23 dickey Exp $";
#endif

/*
 * Title:	ss_field.c
 * Author:	T.E.Dickey
 * Created:	03 Feb 1992
 * Modified:
 *		02 Aug 1992, 'get_field_of()' uses DYN-strings.
 *
 * Function:	insert/extract data from in-memory spreadsheet-table.
 */

#include "spreadsheet.h"

char *	get_ss_field(
	_ARX(char **,	vec)
	_ARX(int,	row)
	_ARX(int,	col)
	_ARX(char *,	dftval)
	_AR1(DYN **,	result)
		)
	_DCL(char **,	vec)
	_DCL(int,	row)
	_DCL(int,	col)
	_DCL(char *,	dftval)
	_DCL(DYN **,	result)
{
	return (vec != 0)
		? get_field_of(vec[row], col, dftval, result)
		: dftval;
}

char *	set_ss_field(
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
