/*
 * Title:	dyn_append.c
 * Author:	T.E.Dickey
 * Created:	07 Feb 1992
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		30 May 1998, compile with g++
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	appends an ordinary string to a dynamic-string.
 */

#define	STR_PTYPES
#include "ptypes.h"
#include "dyn_str.h"

MODULE_ID("$Id: dyn_app.c,v 12.6 2014/12/28 01:11:07 tom Exp $")

DYN *
dyn_append(DYN * p, const char *s)
{
    if (s != 0) {
	size_t len = strlen(s);

	p = dyn_alloc(p, dyn_length(p) + len + 1);
	(void) strcpy(p->text + p->cur_length, s);
	p->cur_length += len;
    }
    return p;
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
