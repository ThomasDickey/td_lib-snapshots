#ifndef	lint
static	char	Id[] = "$Id: zone2vms.c,v 4.0 1989/10/23 09:37:46 ste_cm Rel $";
#endif	lint

/*
 * Title:	zone2vms.c
 * Author:	T.E.Dickey
 * Created:	30 Sep 1988
 *
 * Function:	DEC's unix-like time routines do not account for time-zone or
 *		for daylight-savings time.  Returns an adjustment to a unix-
 *		derived time (GMT) to EST5EDT.  This adjustment is used to
 *		convert a unix-time to VMS wall-clock time.
 *
 *		Note that this is not correct for the switch dates between
 *		00:00 and 02:00!
 */

#include	"portunix.h"

#define	MIN	60
#define	HOUR	(60*MIN)
#define	DAY	(24*HOUR)

#define	M_APR	3
#define	M_OCT	9
#define	D_APR	30
#define	D_OCT	31

time_t
zone2vms(unix_time)
time_t	unix_time;
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
main()
{
	extern	char	*ctime();
	auto	time_t	unix_time = time(0),
			real_time;
	register int	j;

	for (j = 0; j < 27; j++) {
		real_time = unix_time + zone2vms(unix_time);
		printf("%2d) %s", j, ctime(&real_time));
		unix_time -= (DAY * 14);
	}
}
#endif	TEST
