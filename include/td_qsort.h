/* $Id: td_qsort.h,v 12.10 2004/03/07 21:45:58 tom Exp $ */

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

#define	ANSI_QSORT 1
#define	QSORT_CAST(s,d)	register QSORT_DST d = (QSORT_DST) s
#define QSORT_dst const V_OR_P
#define QSORT_DST QSORT_SRC const *
#define	QSORT_FUNC_ARGS(p1,p2)	QSORT_dst q1, QSORT_dst q2

#define	QSORT_FUNC(func) \
	int	func(QSORT_FUNC_ARGS(p1,p2))

#ifndef	LINTLIBRARY
#if	defined(HAVE_QSORT) && defined(NEED_QSORT)
extern	void	qsort(
		V_OR_P	base,
		size_t	nel,
		size_t	width,
		int (*compar)	(QSORT_FUNC_ARGS(a,b)));
#endif
#endif

extern	int	cmp_qsort(
		QSORT_dst	a,
		QSORT_dst	b
		)
		;

#endif/*TD_QSORT_H*/
