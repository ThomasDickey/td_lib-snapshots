/*
 * Title:	saves_uid.c
 * Author:	T.E.Dickey
 * Created:	21 Dec 1992
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	returns true if the operating system supports saved-uid in
 *		set-uid code.  This is a Posix characteristic.
 */

#include "ptypes.h"

MODULE_ID("$Id: savesuid.c,v 12.4 2014/12/28 01:11:01 tom Exp $")

int
saves_uid(void)
{
#ifdef	apollo
    return FALSE;		/* ..at least thru SR10.3.5 */
#else
    return TRUE;		/* patch: SunOs */
#endif
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
