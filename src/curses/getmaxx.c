/*
 * Title:	getmaxx.c (compatible functions for getmaxx, etc)
 * Author:	T.E.Dickey
 * Created:	05 Nov 2003
 */
#include	"td_curse.h"

MODULE_ID("$Id: getmaxx.c,v 12.1 2003/11/06 01:26:36 tom Exp $")

#ifdef HAVE_GETMAXYX

#ifndef HAVE_GETMAXX
#undef getmaxx
int
getmaxx(WINDOW *win)
{
    int y, x;
    getmaxyx(win, y, x);
    return x;
}
#endif

#ifndef HAVE_GETMAXY
#undef getmaxy
int
getmaxy(WINDOW *win)
{
    int y, x;
    getmaxyx(win, y, x);
    return y;
}
#endif

#endif /* HAVE_GETMAXYX */

#ifdef HAVE_GETBEGYX

#ifndef HAVE_GETBEGX
#undef getbegx
int
getbegx(WINDOW *win)
{
    int y, x;
    getmaxyx(win, y, x);
    return x;
}
#endif

#ifndef HAVE_GETBEGY
#undef getbegy
int
getbegy(WINDOW *win)
{
    int y, x;
    getmaxyx(win, y, x);
    return y;
}
#endif

#endif /* HAVE_GETBEGYX */
