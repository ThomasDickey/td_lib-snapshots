/*
 * Title:	dyn_copy.c
 * Author:	T.E.Dickey
 * Created:	03 Apr 1992
 * Modified:
 *		06 Dec 2019, don't copy if the source/target are the same.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		30 May 1998, compile with g++
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	copies an ordinary string to a dynamic-string.
 */

#define	STR_PTYPES
#include "ptypes.h"
#include "dyn_str.h"

MODULE_ID("$Id: dyn_copy.c,v 12.8 2025/01/07 00:14:07 tom Exp $")

DYN *
dyn_copy(DYN * p, const char *s)
{
    size_t len = (s != NULL) ? strlen(s) : 0;

    if (len != 0) {
	p = dyn_alloc(p, len + 1);
	if (p->text != s)
	    (void) strcpy(p->text, s);
	p->cur_length = len;
    } else
	dyn_init(&p, len + 1);
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
