/* $Id: td_qsort.h,v 12.8 1998/05/30 11:44:12 tom Exp $ */

#ifndef	TD_QSORT_H
#define	TD_QSORT_H

#ifndef	PTYPES_H
#include <ptypes.h>
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
#define	QSORT_SRC	const char *
#endif

#if	PROTOTYPES
#define	ANSI_QSORT 1
#define	QSORT_CAST(s,d)	register QSORT_DST d = (QSORT_DST) s;
#define QSORT_dst const V_OR_P
#define QSORT_DST QSORT_SRC const *
#define	QSORT_FUNC_ARGS(p1,p2)	_ARX(QSORT_dst,q1) _AR1(QSORT_dst,q2)
#else
#define	ANSI_QSORT 0
#define	QSORT_CAST(s,d)
#define QSORT_dst QSORT_DST
#define QSORT_DST QSORT_SRC *
#define	QSORT_FUNC_ARGS(p1,p2)	_ARX(QSORT_DST,p1) _AR1(QSORT_DST,p2)
#endif

#define	QSORT_FUNC_DCLS(p1,p2)	_DCL(QSORT_DST,p1) _DCL(QSORT_DST,p2)

#define	QSORT_FUNC(func) \
	int	func(QSORT_FUNC_ARGS(p1,p2)) \
		     QSORT_FUNC_DCLS(p1,p2)

#ifndef	LINTLIBRARY
#if	HAVE_QSORT && NEED_QSORT
extern	void	qsort(
		_arx(V_OR_P,	base)
		_arx(size_t,	nel)
		_arx(size_t,	width)
		_fn1(int,	compar,	(QSORT_FUNC_ARGS(a,b))));
#endif
#endif

extern	int	cmp_qsort(
		_arx(QSORT_dst,	a)
		_ar1(QSORT_dst,	b)
		)
		_dcl(QSORT_DST,	a)
		_dcl(QSORT_DST,	b)
		_ret

#endif/*TD_QSORT_H*/
