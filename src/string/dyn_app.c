/*
 * Title:	dyn_append.c
 * Author:	T.E.Dickey
 * Created:	07 Feb 1992
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	appends an ordinary string to a dynamic-string.
 */

#define	STR_PTYPES
#include "ptypes.h"
#include "dyn_str.h"

MODULE_ID("$Id: dyn_app.c,v 12.2 1993/10/29 17:35:22 tom Exp $")

DYN *	dyn_append(
_ARX(DYN *,	p)
_AR1(char *,	s)
	)
_DCL(DYN *,	p)
_DCL(char *,	s)
{
	if (s != 0) {
		size_t	new	= strlen(s);

		p = dyn_alloc(p, dyn_length(p) + new + 1);
		(void)strcpy(p->text + p->cur_length, s);
		p->cur_length += new;
	}
	return p;
}
