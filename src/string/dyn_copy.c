/*
 * Title:	dyn_copy.c
 * Author:	T.E.Dickey
 * Created:	03 Apr 1992
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		30 May 1998, compile with g++
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	copies an ordinary string to a dynamic-string.
 */

#define	STR_PTYPES
#include "ptypes.h"
#include "dyn_str.h"

MODULE_ID("$Id: dyn_copy.c,v 12.4 2004/03/07 22:03:45 tom Exp $")

DYN *
dyn_copy(DYN * p, char *s)
{
    size_t len = (s != 0) ? strlen(s) : 0;

    if (len != 0) {
	p = dyn_alloc(p, len + 1);
	(void) strcpy(p->text, s);
	p->cur_length = len;
    } else
	dyn_init(&p, len + 1);
    return p;
}
