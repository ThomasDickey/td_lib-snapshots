/*
 * Title:	dyn_catarg.c
 * Author:	T.E.Dickey
 * Created:	07 Feb 1992
 * Modified:
 *		30 May 1998, compile with g++
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	provides dynamic-string interface to 'catarg()'.
 */

#define	STR_PTYPES
#include "ptypes.h"
#include "dyn_str.h"

MODULE_ID("$Id: dyn_cata.c,v 12.3 1998/05/30 10:51:39 tom Exp $")

#define	EMPTY(s)	((s == 0) || (*s == EOS))

DYN *
dyn_catarg(
_ARX(DYN *,	p)
_AR1(char *,	arg)
	)
_DCL(DYN *,	p)
_DCL(char *,	arg)
{
	if (!EMPTY(arg)) {
		size_t	len = strlen(arg) + 2;	/* room for space & null */

		p = dyn_alloc(p, dyn_length(p) + len);
		catarg(dyn_string(p), arg);
		p->cur_length = strlen(p->text);
	}
	return p;
}

DYN *
dyn_catarg2(
_ARX(DYN *,	p)
_ARX(char *,	opt)
_AR1(char *,	value)
	)
_DCL(DYN *,	p)
_DCL(char *,	opt)
_DCL(char *,	value)
{
	if (!EMPTY(value)) {
		size_t	len = strlen(opt) + strlen(value) + 2;

		p = dyn_alloc(p, dyn_length(p) + len);
		catarg2(dyn_string(p), opt, value);
		p->cur_length = strlen(p->text);
	}
	return p;
}
