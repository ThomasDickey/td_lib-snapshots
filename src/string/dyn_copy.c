#ifndef	lint
static	char	*Id = "$Id: dyn_copy.c,v 11.0 1992/04/03 07:41:58 ste_cm Rel $";
#endif

/*
 * Title:	dyn_copy.c
 * Author:	T.E.Dickey
 * Created:	03 Apr 1992
 *
 * Function:	copies an ordinary string to a dynamic-string.
 */

#define	STR_PTYPES
#include "ptypes.h"
#include "dyn_string.h"

DYN *	dyn_copy(
_ARX(DYN *,	p)
_AR1(char *,	s)
	)
_DCL(DYN *,	p)
_DCL(char *,	s)
{
	size_t	new	= (s != 0) ? strlen(s) : 0;

	dyn_init(&p, new+1);
	if (new != 0) {
		(void)strcpy(p->text, s);
		p->cur_length = new;
	}
	return p;
}
