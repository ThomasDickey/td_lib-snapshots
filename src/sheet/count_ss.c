#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: count_ss.c,v 12.1 1993/10/29 17:35:22 dickey Exp $";
#endif

/*
 * Title:	count_ss.c
 * Author:	T.E.Dickey
 * Created:	03 Feb 1992
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	counts rows/columns in a spreadsheet-table (in memory via
 *		'file2argv()').
 */

#include "td_sheet.h"


int
count_ss_cols(
_AR1(char **,	vec))
_DCL(char **,	vec)
{
	auto	int	count	= 0;
	static	DYN	*temp;

	if (vec && *vec)
		while (get_field_of(*vec, count, (char *)0, &temp))
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
