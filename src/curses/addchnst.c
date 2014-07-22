/*
 * Title:	addchnst.c (addchnstr)
 * Author:	T.E.Dickey
 * Created:	28 Jan 1995
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *
 * Function:	Supplies the function 'waddchnstr()' for implementations of
 *		curses that don't have it (e.g., bsd curses, HPUX, AIX).
 *
 *		This leaves the current position after the end of the string.
 *		Some implementations of waddchnstr() leave the position at
 *		the original position.
 */

#include	"ptypes.h"
#include	"td_curse.h"

MODULE_ID("$Id: addchnst.c,v 12.7 2014/07/22 13:51:29 tom Exp $")

#ifndef OK
#define OK 0
#endif

#if !defined(HAVE_ADDCHNSTR)
int
waddchnstr(WINDOW *win, chtype *s, int len)
{
    int code, y, x;

    getyx(win, y, x);
    if (len < 0) {
	for (len = 0; s[len] != 0; len++) ;
    }
    while (len-- > 0) {
	if ((code = waddch(win, *s++)) != OK) {
	    return code;
	}
	if (++x >= wMaxX(win)) {
	    x = wBegX(win);
	    y++;
	}
	wmove(win, y, x);
    }
    return OK;
}
#else
void dummy_addchnst(void);
void
dummy_addchnst(void)
{
}
#endif
