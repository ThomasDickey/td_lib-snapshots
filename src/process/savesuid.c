#ifndef	lint
static	char	*Id = "$Id: savesuid.c,v 12.0 1992/12/21 09:52:53 ste_cm Rel $";
#endif

/*
 * Title:	saves_uid.c
 * Author:	T.E.Dickey
 * Created:	21 Dec 1992
 *
 * Function:	returns true if the operating system supports saved-uid in
 *		set-uid code.  This is a Posix characteristic.
 */

#include "ptypes.h"

int	saves_uid(_AR0)
{
#ifdef	apollo
	return FALSE;	/* ..at least thru SR10.3.5 */
#else
	return TRUE;	/* patch: SunOs */
#endif
}
