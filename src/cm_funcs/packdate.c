#ifndef	lint
static	char	sccs_id[] = "@(#)packdate.c	1.3 86/10/07 15:39:55";
#endif	lint

/*
 * Author:	T.E.Dickey
 *
 * Function:	Given a date, in unpacked form, return the unix time (GMT
 *		seconds since 1 Jan 1970) which will produce an equivalent
 *		*local time*.
 *
 *		Base date is Thu Jan 1, 1970
 */

#include	<time.h>
extern	struct	tm	*localtime();
extern	long	timezone;

#define	MINUTE	60
#define	HOUR	(60*MINUTE)
#define	DAY	(24*HOUR)
#define	YEAR	(365*DAY)

#define	LEAP(y)	(!(y&3))

long	packdate (year, mon, day, hour, min, s)
{
long	sec = s;
register int	j;
static	int	m[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
/*			  jan,feb,mar,apr,may,jun,jul,aug,sep,oct,nov,dec */

	sec += timezone + 60 * (min + 60 * hour);
	for (j = 1970; j < year; j++) {
		sec += YEAR;
		if (LEAP(j))	sec += DAY;
	}
	for (j = 1; j < mon; j++) {
		sec += DAY * m[j-1];
	}
	if (mon >= 2 && LEAP(year))	sec += DAY;
	sec += (day-1) * DAY;
	if (dst(sec))	sec -= HOUR;
	return (sec);
}

/*
 * Check to see if the local-time display for a given clock-time (in GMT
 * seconds) would be bumped up an hour for daylight savings time.  This uses
 * the procedure 'localtime' to avoid the complication of determining the
 * range between the last-Sunday-in-April to the last-Sunday-in-October!
 */
static	dst (clock)
long	clock;
{
struct	tm	tm;
	tm = *localtime(&clock);
	return (tm.tm_isdst);
}
