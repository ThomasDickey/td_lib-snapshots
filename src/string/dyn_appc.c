/*
 * Title:	dyn_append_c.c
 * Author:	T.E.Dickey
 * Created:	24 Jul 1992
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	appends a character to a dynamic-string.
 */

#define	STR_PTYPES
#include "ptypes.h"
#include "dyn_str.h"

MODULE_ID("$Id: dyn_appc.c,v 12.2 1993/10/29 17:35:22 tom Exp $")

DYN *	dyn_append_c(
	_ARX(DYN *,	p)
	_AR1(int,	c)
		)
	_DCL(DYN *,	p)
	_DCL(int,	c)
{
	static	char	temp[] = "?";
	if (c != EOS) {
		temp[0] = c;
		p = dyn_append(p, temp);
	}
	return p;
}
