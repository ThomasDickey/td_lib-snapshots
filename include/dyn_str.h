/* $Id: dyn_str.h,v 10.1 1992/02/06 15:53:48 dickey Exp $
 *
 *	Functions which manage dynamic-strings.
 */
#ifndef	_DYN_STRING_H_
#define	_DYN_STRING_H_

#ifndef	_arx
#include "ptypes.h"
#endif

typedef	struct	{
	char	*text;
	int	cur_length,
		max_length;
	} DYN;

	/* dyn_append.c ----------------------------------------------- */
	DYN *	dyn_append(
		_arx(DYN *,	p)
		_ar1(char *,	s)
		)
		_dcl(DYN *,	p)
		_dcl(char *,	s)
		_ret

	/* dyn_catarg.c ----------------------------------------------- */
	DYN *	dyn_catarg(
		_ar1(DYN *,	p)
		)
		_dcl(DYN *,	p)
		_ret

	DYN *	dyn_catarg2(
		_ar1(DYN *,	p)
		)
		_dcl(DYN *,	p)
		_ret

	/* dyn_string.c ----------------------------------------------- */
	DYN *	dyn_alloc(
		_arx(DYN *,	p)
		_ar1(size_t,	len)
		)
		_dcl(DYN *,	p)
		_dcl(size_t,	len)
		_ret

	DYN *	dyn_free(
		_ar1(DYN **,	p)
		)
		_dcl(DYN **,	p)
		_ret

	void	dyn_clear(
		_ar1(DYN *,	p)
		)
		_dcl(DYN *,	p)
		_nul

	char *	dyn_string(
		_ar1(DYN *,	p)
		)
		_dcl(DYN *,	p)
		_ret

	int	dyn_length(
		_ar1(DYN *,	p)
		)
		_dcl(DYN *,	p)
		_ret

#endif	/* _DYN_STRING_H_ */
