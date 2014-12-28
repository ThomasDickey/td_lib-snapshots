/*
 * Title:	is_xterm.c
 * Author:	T.E.Dickey
 * Created:	30 Jul 1995 (from a note in 'tin')
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *
 * Function:	returns true if the terminal is an xterm (or equivalent) 
 */
#define STR_PTYPES
#include "td_curse.h"

MODULE_ID("$Id: is_xterm.c,v 12.6 2014/12/28 01:10:44 tom Exp $")

#if !defined(HAVE_IS_XTERM)
/*
 * The most reliable way to determine if we're really running in an xterm is to
 * try to open the X Window display.  That has the drawback that we've got to
 * link with the X11 library.  Other options include seeing if the $TERM
 * variable is set to something containing "xterm" (what about mterm, hpterm),
 * or using one of the special escape sequences for xterm.
 */
int
is_xterm(void)
{
    static int initialized = FALSE;
    static int result = FALSE;
    if (!initialized) {
	char *term = getenv("TERM");
	initialized = TRUE;
	if (term != 0) {
	    if (!strncmp(term, "mterm", (size_t) 5)
		|| !strncmp(term, "hpterm", (size_t) 6)) {
		result = TRUE;
	    } else {
		while (*term != EOS) {
		    if (!strncmp(term++, "xterm", (size_t) 5)) {
			result = TRUE;
			break;
		    }
		}
	    }
	}
    }
    return result;
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
