/*
 * Title:	zone2vms.c
 * Author:	T.E.Dickey
 * Created:	30 Sep 1988
 * Modified:
 *		01 Dec 1993, ifdefs.
 *		20 Nov 1992, use prototypes
 *
 * Function:	DEC's unix-like time routines do not account for time-zone or
 *		for daylight-savings time.  Returns an adjustment to a unix-
 *		derived time (GMT) to EST5EDT.  This adjustment is used to
 *		convert a unix-time to VMS wall-clock time.
 *
 *		Note that this is not correct for the switch dates between
 *		00:00 and 02:00!
 */

#define TIM_PTYPES
#include	"port2vms.h"

MODULE_ID("$Id: zone2vms.c,v 12.5 1995/10/31 23:23:12 tom Exp $")

#define	M_APR	3
#define	M_OCT	9
#define	D_APR	30
#define	D_OCT	31

time_t	zone2vms(
	_AR1(time_t,	unix_time))
	_DCL(time_t,	unix_time)
{
	auto	struct tm   mytm;
	auto	int	sunday;

	mytm	= *localtime(&unix_time);
	sunday	= mytm.tm_mday - mytm.tm_wday;

	unix_time = 0;			/* ...will be the adjustment */
	if (mytm.tm_mon == M_APR) {	/* april (first/last, by year) */
		if ((mytm.tm_year >= 86 && (sunday >= 0))
		||  (mytm.tm_year <= 85 && (sunday + 7 >= D_APR)))
			mytm.tm_isdst = 1;
	} else if (mytm.tm_mon == M_OCT) { /* october (last sunday) */
		if (sunday + 7 < D_OCT)
			mytm.tm_isdst = 1;
	} else if (mytm.tm_mon > M_APR && mytm.tm_mon < M_OCT)
		mytm.tm_isdst = 1;

	if (mytm.tm_isdst)
		unix_time += HOUR;
	unix_time -= 5 * HOUR;		/* adjust to EST */
	return (unix_time);
}

#ifdef	TEST
/*ARGSUSED*/
_MAIN
{
	auto	time_t	unix_time = 722271384/* time(0) for Nov 20, 1992 */,
			real_time;
	register int	j;

	for (j = 0; j < 27; j++) {
		real_time = unix_time + zone2vms(unix_time);
		PRINTF("%2d) %s", j, ctime(&real_time));
		unix_time -= (DAY * 14);
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	/* TEST */
