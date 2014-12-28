/*
 * Title:	ss_field.c
 * Author:	T.E.Dickey
 * Created:	03 Feb 1992
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		02 Aug 1992, 'get_field_of()' uses DYN-strings.
 *
 * Function:	insert/extract data from in-memory spreadsheet-table.
 */

#include "td_sheet.h"

MODULE_ID("$Id: ss_field.c,v 12.4 2014/12/28 01:11:04 tom Exp $")

char *
get_ss_field(char **vec,
	     int row,
	     int col,
	     char *dftval,
	     DYN ** result)
{
    return ((vec != 0)
	    ? get_field_of(vec[row], col, dftval, result)
	    : dftval);
}

char *
set_ss_field(char **vec,
	     int row,
	     int col,
	     char *buffer)
{
    return set_field_of(vec[row], col, buffer);
}

/******************************************************************************/
#ifdef	TEST
_MAIN
{
    (void) argc;
    (void) argv;
    exit(EXIT_FAILURE);
    /*NOTREACHED */
}
#endif /* TEST */
