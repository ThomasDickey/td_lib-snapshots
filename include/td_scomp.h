/* $Id: td_scomp.h,v 12.5 2004/03/07 21:50:03 tom Exp $ */

#ifndef	TD_SCOMP_H
#define	TD_SCOMP_H
#include	<ptypes.h>

/*
 * Define types used to encapsulate difference algorithms.  These assume that
 * the files/data to be compared reside in memory.
 */

#ifndef	SCOMP
#define	SCOMP	m2comp		/* current default comparison algorithm */
#endif

#ifndef	SCOMP_TYPE
#define	SCOMP_TYPE	char **
#endif

#define	SCOMP_MATCH_ARGS(p1,p2)\
		SCOMP_TYPE p1,\
		SCOMP_TYPE p2

#define	SCOMP_REPORT_ARGS(v1,v2)\
		SCOMP_TYPE	v1,\
		int		lo_1,\
		int		hi_1,\
		SCOMP_TYPE	v2,\
		int		lo_2,\
		int		hi_2

#define	SCOMP_CALL(func)\
	void	func(\
		SCOMP_TYPE	v1,\
		int		n1,\
		SCOMP_TYPE	v2,\
		int		n2,\
		int		size,\
		int (*match)	(SCOMP_MATCH_ARGS(p1,p2)),\
		void (*report)	(SCOMP_REPORT_ARGS(p1,p2))\
		)\
		;

#define	SCOMP_MATCH(func)\
	int	func(SCOMP_MATCH_ARGS(p1,p2))

#define	SCOMP_REPORT(func)\
	void	func(SCOMP_REPORT_ARGS(v1,v2))

extern	SCOMP_CALL(scomp)
extern	SCOMP_CALL(m2comp)

#endif/*TD_SCOMP_H*/
