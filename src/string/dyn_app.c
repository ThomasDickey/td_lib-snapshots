#ifndef	lint
static	char	*Id = "$Id: dyn_app.c,v 12.0 1993/04/26 16:16:26 ste_cm Rel $";
#endif

/*
 * Title:	dyn_append.c
 * Author:	T.E.Dickey
 * Created:	07 Feb 1992
 *
 * Function:	appends an ordinary string to a dynamic-string.
 */

#define	STR_PTYPES
#include "ptypes.h"
#include "dyn_str.h"

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
