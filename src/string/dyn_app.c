/*
 * Title:	dyn_append.c
 * Author:	T.E.Dickey
 * Created:	07 Feb 1992
 * Modified:
 *		30 May 1998, compile with g++
 *		29 Oct 1993, ifdef-ident
 *
 * Function:	appends an ordinary string to a dynamic-string.
 */

#define	STR_PTYPES
#include "ptypes.h"
#include "dyn_str.h"

MODULE_ID("$Id: dyn_app.c,v 12.3 1998/05/30 10:51:39 tom Exp $")

DYN *	dyn_append(
_ARX(DYN *,	p)
_AR1(char *,	s)
	)
_DCL(DYN *,	p)
_DCL(char *,	s)
{
	if (s != 0) {
		size_t	len	= strlen(s);

		p = dyn_alloc(p, dyn_length(p) + len + 1);
		(void)strcpy(p->text + p->cur_length, s);
		p->cur_length += len;
	}
	return p;
}
