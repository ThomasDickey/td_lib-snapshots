#ifndef	lint
static	char	*Id = "$Id: dyn_str.c,v 11.0 1992/02/07 11:15:52 ste_cm Rel $";
#endif

/*
 * Title:	dyn_string.c
 * Author:	T.E.Dickey
 * Created:	07 Feb 1992
 *
 * Function:	manages dynamic strings.  These are used for constructing
 *		arbitrarily long strings, especially for argument-lists to
 *		be passed to other processes via 'execute()'.
 */

#include "ptypes.h"
#include "dyn_string.h"

#define	def_alloc	DYN_ALLOC

	/*ARGSUSED*/
	def_ALLOC(DYN)

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

/*
 * Ensures that the maximum-length for the given dyn-string is at least that
 * given as an argument.
 */
DYN *
dyn_alloc(
_ARX(DYN *,	p)
_AR1(size_t,	len)
	)
_DCL(DYN *,	p)
_DCL(size_t,	len)
{
	if (p == 0) {
		static	DYN	empty;
		*(p = ALLOC(DYN,1)) = empty;
	}

	if (p->max_length < len) {
		p->max_length = (len * 5)/4;
		p->text = doalloc(p->text, (unsigned)(p->max_length));
		p->text[p->cur_length] = EOS;
	}
	return p;
}

/*
 * Free all storage associated with a dyn-string.
 */
DYN *
dyn_free(
_AR1(DYN *,	p))
_DCL(DYN *,	p)
{
	if (p != 0) {
		if (p->text != 0)
			dofree(p->text);
		dofree((char *)p);
	}
	return 0;
}

/*
 * Ensure that the given dyn-string is empty.
 */
void
dyn_init(
_ARX(DYN **,	p)
_AR1(size_t,	len)
	)
_DCL(DYN **,	p)
_DCL(size_t,	len)
{
	DYN	*q = dyn_alloc(*p, len);
	q->text[q->cur_length = 0] = EOS;
	*p = q;
}

/*
 * Return the string-portion of the dyn-string.
 */
char *
dyn_string(
_AR1(DYN *,	p))
_DCL(DYN *,	p)
{
	return (p != 0) ? p->text : 0;
}

/*
 * Returns the current-length of the dyn-string.
 */
size_t
dyn_length(
_AR1(DYN *,	p))
_DCL(DYN *,	p)
{
	return (p != 0) ? p->cur_length : 0;
}
