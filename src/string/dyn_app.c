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

MODULE_ID("$Id: dyn_app.c,v 12.4 2004/03/07 22:03:45 tom Exp $")

DYN *
dyn_append(DYN * p, char *s)
{
    if (s != 0) {
	size_t len = strlen(s);

	p = dyn_alloc(p, dyn_length(p) + len + 1);
	(void) strcpy(p->text + p->cur_length, s);
	p->cur_length += len;
    }
    return p;
}
