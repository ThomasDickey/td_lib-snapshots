/*
 * Title:	count_ss.c
 * Author:	T.E.Dickey
 * Created:	03 Feb 1992
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	counts rows/columns in a spreadsheet-table (in memory via
 *		'file2argv()').
 */

#include "td_sheet.h"

MODULE_ID("$Id: count_ss.c,v 12.3 2004/03/07 22:03:45 tom Exp $")

int
count_ss_cols(char **vec)
{
    int count = 0;
    static DYN *temp;

    if (vec && *vec)
	while (get_field_of(*vec, count, (char *) 0, &temp))
	    count++;
    return count;
}

int
count_ss_rows(char **vec)
{
    return veclen(vec);
}
