#ifndef	lint
static	char	*Id = "$Id: dyn_copy.c,v 11.1 1992/09/02 08:19:30 dickey Exp $";
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
	register size_t	new	= (s != 0) ? strlen(s) : 0;

	if (new != 0) {
		p = dyn_alloc(p, new+1);
		(void)strcpy(p->text, s);
		p->cur_length = new;
	} else
		dyn_init(&p, new+1);
	return p;
}
