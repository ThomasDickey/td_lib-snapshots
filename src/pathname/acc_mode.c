#ifndef	lint
static	char	*Id = "$Id: acc_mode.c,v 10.3 1992/02/07 14:43:29 dickey Exp $";
#endif

/*
 * Title:	access_mode.c
 * Author:	T.E.Dickey
 * Created:	07 Feb 1992
 *
 * Function:	converts the mode-argument to 'access()' into a printable
 *		string.
 */

#define	ACC_PTYPES
#include "ptypes.h"

char *
access_mode(
_AR1(int,	mode))
_DCL(int,	mode)
{
	static	char	value[16];

	if (mode == F_OK)
		(void)strcpy(value, "F");
	else if (mode > 7 || mode < 0)
		FORMAT(value, "?%d", mode);
	else {
		*value = EOS;
		if (mode & R_OK)	(void)strcat(value, "R");
		if (mode & W_OK)	(void)strcat(value, "W");
		if (mode & X_OK)	(void)strcat(value, "X");
	}
	return value;
}
