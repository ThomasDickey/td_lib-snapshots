/*
 * Title:	dyn_catarg.c
 * Author:	T.E.Dickey
 * Created:	07 Feb 1992
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		30 May 1998, compile with g++
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	provides dynamic-string interface to 'catarg()'.
 */

#define	STR_PTYPES
#include "ptypes.h"
#include "dyn_str.h"

MODULE_ID("$Id: dyn_cata.c,v 12.5 2010/07/03 16:15:09 tom Exp $")

#define	EMPTY(s)	((s == 0) || (*s == EOS))

DYN *
dyn_catarg(DYN * p, const char *arg)
{
    if (!EMPTY(arg)) {
	size_t len = strlen(arg) + 2;	/* room for space & null */

	p = dyn_alloc(p, dyn_length(p) + len);
	catarg(dyn_string(p), arg);
	p->cur_length = strlen(p->text);
    }
    return p;
}

DYN *
dyn_catarg2(DYN * p, const char *opt, const char *value)
{
    if (!EMPTY(value)) {
	size_t len = strlen(opt) + strlen(value) + 2;

	p = dyn_alloc(p, dyn_length(p) + len);
	catarg2(dyn_string(p), opt, value);
	p->cur_length = strlen(p->text);
    }
    return p;
}
