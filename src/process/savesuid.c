/*
 * Title:	saves_uid.c
 * Author:	T.E.Dickey
 * Created:	21 Dec 1992
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	returns true if the operating system supports saved-uid in
 *		set-uid code.  This is a Posix characteristic.
 */

#include "ptypes.h"

MODULE_ID("$Id: savesuid.c,v 12.2 1993/10/29 17:35:23 tom Exp $")

int	saves_uid(_AR0)
{
#ifdef	apollo
	return FALSE;	/* ..at least thru SR10.3.5 */
#else
	return TRUE;	/* patch: SunOs */
#endif
}
