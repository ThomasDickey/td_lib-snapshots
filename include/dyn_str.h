/* $Id: dyn_str.h,v 12.1 1994/06/25 14:15:54 tom Exp $
 *
 *	Functions which manage dynamic-strings.
 */

#ifndef	DYN_STR_H
#define	DYN_STR_H

#ifndef	_arx
#include "ptypes.h"
#endif

typedef	struct	{
	char	*text;
	size_t	cur_length,
		max_length;
	} DYN;

#define	APPEND	(void) dyn_append
#define	CATARG	(void) dyn_catarg
#define	CATARG2	(void) dyn_catarg2

	/* dyn_append.c ----------------------------------------------- */
	DYN *	dyn_append(
		_arx(DYN *,	p)
		_ar1(char *,	s)
		)
		_dcl(DYN *,	p)
		_dcl(char *,	s)
		_ret

	/* dyn_append_c.c --------------------------------------------- */
	DYN *	dyn_append_c(
		_arx(DYN *,	p)
		_ar1(int,	c)
		)
		_dcl(DYN *,	p)
		_dcl(int,	c)
		_ret

	/* dyn_catarg.c ----------------------------------------------- */
	DYN *	dyn_catarg(
		_arx(DYN *,	p)
		_ar1(char *,	arg)
		)
		_dcl(DYN *,	p)
		_dcl(char *,	arg)
		_ret

	DYN *	dyn_catarg2(
		_arx(DYN *,	p)
		_arx(char *,	opt)
		_ar1(char *,	value)
		)
		_dcl(DYN *,	p)
		_dcl(char *,	opt)
		_dcl(char *,	value)
		_ret

	/* dyn_copy.c ------------------------------------------------- */
	DYN *	dyn_copy(
		_arx(DYN *,	p)
		_ar1(char *,	s)
		)
		_dcl(DYN *,	p)
		_dcl(char *,	s)
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
		_ar1(DYN *,	p)
		)
		_dcl(DYN *,	p)
		_ret

	void	dyn_init(
		_arx(DYN **,	p)
		_ar1(size_t,	len)
		)
		_dcl(DYN **,	p)
		_dcl(size_t,	len)
		_nul

	char *	dyn_string(
		_ar1(DYN *,	p)
		)
		_dcl(DYN *,	p)
		_ret

	size_t	dyn_length(
		_ar1(DYN *,	p)
		)
		_dcl(DYN *,	p)
		_ret

#endif	/* DYN_STR_H */
