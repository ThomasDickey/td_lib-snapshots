#ifndef	lint
static	char	*Id = "$Id: dyn_appc.c,v 12.0 1993/04/26 16:17:43 ste_cm Rel $";
#endif

/*
 * Title:	dyn_append_c.c
 * Author:	T.E.Dickey
 * Created:	24 Jul 1992
 *
 * Function:	appends a character to a dynamic-string.
 */

#define	STR_PTYPES
#include "ptypes.h"
#include "dyn_str.h"

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
