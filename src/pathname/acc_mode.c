#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: acc_mode.c,v 12.4 1994/10/07 00:06:13 tom Exp $";
#endif

/*
 * Title:	access_mode.c
 * Author:	T.E.Dickey
 * Created:	07 Feb 1992
 * Modified:
 *		10 Jul 1994, rephrased w/o strcat.
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	converts the mode-argument to 'access()' into a printable
 *		string.
 */

#define	ACC_PTYPES
#define	STR_PTYPES
#include "ptypes.h"

char *	access_mode(
	_AR1(int,	mode))
	_DCL(int,	mode)
{
	static	char	value[16];
	register char	*s = value;

	if (mode == F_OK) {
		*s++ = 'F';
	} else if (mode > 7 || mode < 0) {
		s = ltostr(value, (long)mode, 8);
	} else {
		if (mode & R_OK)	*s++ = 'R';
		if (mode & W_OK)	*s++ = 'W';
		if (mode & X_OK)	*s++ = 'X';
	}
	*s = EOS;
	return value;
}
