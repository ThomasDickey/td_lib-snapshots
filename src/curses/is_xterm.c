/*
 * Title:	is_xterm.c
 * Author:	T.E.Dickey
 * Created:	30 Jul 1995 (from a note in 'tin')
 * Modified:
 *
 * Function:	returns true if the terminal is an xterm (or equivalent) 
 */
#define STR_PTYPES
#include "td_curse.h"

MODULE_ID("$Id: is_xterm.c,v 12.3 2002/07/05 10:54:18 tom Exp $")

#if !defined(HAVE_IS_XTERM)
/*
 * The most reliable way to determine if we're really running in an xterm is to
 * try to open the X Window display.  That has the drawback that we've got to
 * link with the X11 library.  Other options include seeing if the $TERM
 * variable is set to something containing "xterm" (what about mterm, hpterm),
 * or using one of the special escape sequences for xterm.
 */
int is_xterm(_AR0)
{
	static	int	initialized = FALSE;
	static	int	result = FALSE;
	if (!initialized) {
		char *term = getenv("TERM");
		initialized = TRUE;
		if (term != 0) {
			if (!strncmp(term, "mterm", 5)
			 || !strncmp(term, "hpterm", 6)) {
				result = TRUE;
			} else {
				while (*term != EOS) {
					if (!strncmp(term++, "xterm", 5)) {
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
