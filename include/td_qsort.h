/* $Id: td_qsort.h,v 11.2 1992/11/18 08:32:18 dickey Exp $ */
#ifndef	_CM_QSORT_H
#define	_CM_QSORT_H

#ifndef	PTYPES_H
#include "ptypes.h"
#endif

/*
 * Define the following:
 *
 *	QSORT_SRC -	the type of the array passed to 'qsort()'.  If defined,
 *			we assume this is not the default type, and perform a
 *			cast within the comparison function.
 *
 *	QSORT_DST -	the actual type used in the comparison.  For example, if
 *			we sort an array of strings,
 *				QSORT_SRC == (char *)
 *			and we see
 *				QSORT_DST == (char **) == (QSORT_SRC*)
 *			as the actual type.
 */

#ifndef	QSORT_SRC
#define	QSORT_SRC	char *
#endif

#define	QSORT_DST	QSORT_SRC*

#ifdef	__STDC__
#define	QSORT_CAST(s,d)	register QSORT_DST d = (QSORT_DST) s;
#define	QSORT_FUNC_ARGS(p1,p2)	_ARX(V_OR_P,   q1) _AR1(V_OR_P,   q2)
#else
#define	QSORT_CAST(s,d)
#define	QSORT_FUNC_ARGS(p1,p2)	_ARX(QSORT_DST,p1) _AR1(QSORT_DST,p2)
#endif

#define	QSORT_FUNC_DCLS(p1,p2)	_DCL(QSORT_DST,p1) _DCL(QSORT_DST,p2)

#define	QSORT_FUNC(func) \
	int	func(QSORT_FUNC_ARGS(p1,p2)) \
		     QSORT_FUNC_DCLS(p1,p2)

#ifndef	LINTLIBRARY
extern	V_OR_I	qsort(
		_arx(V_OR_P,	base)
		_arx(size_t,	nel)
		_arx(size_t,	width)
		_fn1(int,	compar,	(QSORT_FUNC_ARGS(a,b))));
#endif

extern	int	cmp_qsort(
		_arx(V_OR_P,	a)
		_ar1(V_OR_P,	b)
		)
		_dcl(QSORT_DST,	a)
		_dcl(QSORT_DST,	b)
		_ret

#endif/*_CM_QSORT_H*/
