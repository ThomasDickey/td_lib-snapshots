/*
 * Title:	beep.c (sound audible alarm)
 * Author:	T.E.Dickey
 * Created:	24 Mar (from 'ded.c')
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		26 Apr 1994, port to Linux
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		11 Aug 1988, since we use this only for curses applications,
 *			     don't use it in system5!
 */

#include	"td_curse.h"

MODULE_ID("$Id: beep.c,v 12.11 2014/12/28 01:10:44 tom Exp $")

#if !defined(HAVE_BEEP)
void
beep(void)
{
    blip('\007');
}

#else
void dummy_beep(void);
void
dummy_beep(void)
{
}
#endif

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
