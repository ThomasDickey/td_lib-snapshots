/*
 * Title:	access_mode.c
 * Author:	T.E.Dickey
 * Created:	07 Feb 1992
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		10 Jul 1994, rephrased w/o strcat.
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	converts the mode-argument to 'access()' into a printable
 *		string.
 */

#define	ACC_PTYPES
#define	STR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: acc_mode.c,v 12.7 2004/03/07 22:03:45 tom Exp $")

char *
access_mode(int mode)
{
    static char value[16];
    char *s = value;

    if (mode == F_OK) {
	*s++ = 'F';
    } else if (mode > 7 || mode < 0) {
	s = l2str(value, (long) mode, 8);
    } else {
	if (mode & R_OK)
	    *s++ = 'R';
	if (mode & W_OK)
	    *s++ = 'W';
	if (mode & X_OK)
	    *s++ = 'X';
    }
    *s = EOS;
    return value;
}
