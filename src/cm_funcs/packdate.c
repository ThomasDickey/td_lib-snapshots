#ifndef	lint
static	char	Id[] = "$Id: packdate.c,v 8.0 1990/06/28 15:10:46 ste_cm Rel $";
#endif	lint

/*
 * Title:	packdate.c (pack numbers to make a UNIX date)
 * Author:	T.E.Dickey
 * Created:	26 Mar 1986
 * $Log: packdate.c,v $
 * Revision 8.0  1990/06/28 15:10:46  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.1  90/06/28  15:10:46  dickey
 *		added a test-driver.
 *		modified (for SunOs 4.1 upgrade) code which adjusts for the
 *		timezone, since the 'gettimeofday()' system call was not
 *		working properly.
 *		
 *		Revision 7.0  89/07/25  09:12:18  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/07/25  09:12:18  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/07/25  09:12:18  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  89/07/25  09:12:18  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.1  89/07/25  09:12:18  dickey
 *		recompiled with apollo SR10 -- mods for function prototypes
 *		
 *		Revision 3.0  88/08/17  09:44:06  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/17  09:44:06  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.12  88/08/17  09:44:06  dickey
 *		sccs2rcs keywords
 *		
 *		06 May 1988, port to gould
 *		01 Feb 1988, ooops: wrong computation for leapyear.
 *		24 Nov 1987, made to run on BSD4.2 (as opposed to SYSTEM5).
 *
 * Function:	Given a date, in unpacked form, return the unix time (GMT
 *		seconds since 1 Jan 1970) which will produce an equivalent
 *		*local time*.
 *
 *		Base date is Thu Jan 1, 1970
 */

#include	"ptypes.h"
#ifdef	SYSTEM5
#include	<time.h>
#else
#include	<sys/time.h>
#endif
extern	struct	tm	*localtime();

#ifdef	TEST
#define	DEBUG(s)	printf s;
#endif

#define	MINUTE	60
#define	HOUR	(60*MINUTE)
#define	DAY	(24*HOUR)
#define	YEAR	(365*DAY)

#define	LEAP(y)	(!(y&3))

long
packdate (year, mon, day, hour, min, s)
int	 year, mon, day, hour, min, s;
{
struct	tm tm;
time_t	sec = s;
register int	j;
static	int	m[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
/*			  jan,feb,mar,apr,may,jun,jul,aug,sep,oct,nov,dec */

	sec += (MINUTE * min) + (HOUR * hour);
	for (j = 1970; j < year; j++) {
		sec += YEAR;
		if (LEAP(j))	sec += DAY;
	}
	for (j = 1; j < mon; j++) {
		sec += DAY * m[j-1];
	}
	if (mon > 2 && LEAP(year))	sec += DAY;
	sec += (day-1) * DAY;
	/* adjustments for timezone, etc. */

	/*
	 * Parse the computed time using 'localtime' to avoid the complication
	 * of determining the range between the last-Sunday-in-April to the
	 * last-Sunday-in-October. Invoke it here so we can bypass the SunOs
	 * 'gettimeofday()' procedure, which does not work well.
	 */
	tm = *localtime(&sec);

#ifdef	SYSTEM5
	{
	extern	long	timezone;
		sec += timezone;
	}
#else	!SYSTEM5
#ifdef	sun
	sec -= tm.tm_gmtoff;
#else	UNIX
	{
	struct	timeval	t;
	struct	timezone tz;
		(void)gettimeofday(&t, &tz);
		sec += (tz.tz_minuteswest * MINUTE);
	}
#endif	sun/UNIX
#endif	SYSTEM5/sun/UNIX

	/*
	 * Check to see if the local-time display for a given clock-time
	 * (in GMT seconds) would be bumped up an hour for daylight savings
	 * time.
	 */
#ifndef	sun
	if (tm.tm_isdst)	sec -= HOUR;
#endif

	return (sec);
}

/************************************************************************
 *	Test Driver							*
 ************************************************************************/
#ifdef	TEST
main(argc,argv)
char	*argv[];
{
	auto	time_t	now = time((time_t *)0);
	auto	time_t	then;
	auto	struct	tm tm;
	tm = *localtime(&now);

	printf("Current time: %s", ctime(&now));
	printf("  sec   =%d\n",  tm.tm_sec);
	printf("  min   =%d\n",  tm.tm_min);
	printf("  hour  =%d\n",  tm.tm_hour);
	printf("  mday  =%d\n",  tm.tm_mday);
	printf("  mon   =%d\n",  tm.tm_mon);
	printf("  year  =%d\n",  tm.tm_year);
	printf("  wday  =%d\n",  tm.tm_wday);
	printf("  yday  =%d\n",  tm.tm_yday);
	printf("  isdst =%d\n",  tm.tm_isdst);
#ifdef	sun
	printf("  zone  =%s\n",  tm.tm_zone);
	printf("  gmtoff=%ld\n", tm.tm_gmtoff);
#endif

	then = packdate (tm.tm_year+1900, tm.tm_mon, tm.tm_mday,
			tm.tm_hour,       tm.tm_min, tm.tm_sec);
	printf("Packed time: %s", ctime(&then));
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	TEST
