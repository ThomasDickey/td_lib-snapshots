/* $Id: td_scomp.h,v 12.1 1993/09/21 18:28:49 dickey Exp $ */

#ifndef	_CM_SCOMP_H
#define	_CM_SCOMP_H
#include	"ptypes.h"

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

#define	SCOMP_MATCH_ARGS \
		_ARX(SCOMP_TYPE,p1)\
		_AR1(SCOMP_TYPE,p2)

#define	SCOMP_REPORT_ARGS \
		_ARX(SCOMP_TYPE,v1x)\
		_ARX(int,	lo_1)\
		_ARX(int,	hi_1)\
		_ARX(SCOMP_TYPE,v2x)\
		_ARX(int,	lo_2)\
		_AR1(int,	hi_2)

#define	SCOMP_CALL(func)\
	void	func(\
		_arx(SCOMP_TYPE,v1)\
		_arx(int,	n1)\
		_arx(SCOMP_TYPE,v2)\
		_arx(int,	n2)\
		_arx(int,	size)\
		_fnx(int,	match,	(SCOMP_MATCH_ARGS))\
		_fn1(void,	report,	(SCOMP_REPORT_ARGS))\
		)\
		_dcl(SCOMP_TYPE,v1)\
		_dcl(int,	n1)\
		_dcl(SCOMP_TYPE,v2)\
		_dcl(int,	n2)\
		_dcl(int,	size)\
		_dcl(int,	(*match)())\
		_dcl(void,	(*report)())\
		_nul

#define	SCOMP_MATCH(func)\
	int	func(SCOMP_MATCH_ARGS)\
		_DCL(SCOMP_TYPE,p1)\
		_DCL(SCOMP_TYPE,p2)

#define	SCOMP_REPORT(func)\
	void	func(SCOMP_REPORT_ARGS)\
		_DCL(SCOMP_TYPE,v1)\
		_DCL(SCOMP_TYPE,v2)

extern	SCOMP_CALL(scomp)
extern	SCOMP_CALL(m2comp)

#endif/*_CM_SCOMP_H*/
