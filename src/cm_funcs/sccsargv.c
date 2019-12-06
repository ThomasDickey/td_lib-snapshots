/*
 * Title:	sccsargv.c (initialize ARGV for sccs-utility)
 * Author:	T.E.Dickey
 * Created:	06 Dec 2019
 * Modified:
 *
 * Function:	This initializes an ARGV for a given sccs utility, taking
 *		into account whether the "sccs" wrapper must be used.
 */

#include	<ptypes.h>
#include	<dyn_str.h>
#include	<sccsdefs.h>

MODULE_ID("$Id: sccsargv.c,v 12.1 2019/12/06 18:55:29 tom Exp $")

ARGV *
sccs_argv(const char *tool)
{
#if defined(SCCS_PATH) && !defined(ADMIN_PATH)
    ARGV *argv = argv_init1(sccspath("sccs"));
    argv_append(&argv, tool);
#else
    ARGV *argv = argv_init1(sccspath(tool));
#endif
    return argv;
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
