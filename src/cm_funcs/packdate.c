/*
 * Title:	packdate.c (pack numbers to make a UNIX date)
 * Author:	T.E.Dickey
 * Created:	26 Mar 1986
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		30 Oct 1992, added entrypoint 'gmt_offset()'
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		26 Jun 1990, added a test-driver.  Modified (for SunOs 4.1
 *			     upgrade) code which adjusts for the timezone,
 *			     since the 'gettimeofday()' system call was not
 *			     working properly.
 *		25 Jul 1989, recompiled with apollo SR10 -- mods for function
 *			     prototypes
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

#define TIM_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: packdate.c,v 12.11 1995/10/14 16:37:14 tom Exp $")

#define	LEAP(y)	(!(y&3))

long	gmt_offset(
	_AR1(time_t,	t))
	_DCL(time_t,	t)
{
	struct	tm tm;
	long	sec	= 0;

	/*
	 * Parse the computed time using 'localtime' to avoid the complication
	 * of determining the range between the last-Sunday-in-April to the
	 * last-Sunday-in-October. Invoke it here so we can bypass the SunOs
	 * 'gettimeofday()' procedure, which does not work well.
	 */
	tm = *localtime(&t);

#if	TIMEZONE_DECLARED
	sec += timezone;
#else
#  if	HAVE_TM_GMTOFF
	sec -= tm.tm_gmtoff;
#  else	/* UNIX */
#    if HAVE_GETTIMEOFDAY
	{
	struct	timeval	t2;
	struct	timezone tz;
		(void)gettimeofday(&t2, &tz);
		sec += (tz.tz_minuteswest * MINUTE);
	}
#    endif
#  endif
#endif	/* TIMEZONE_DECLARED */

	/*
	 * Check to see if the local-time display for a given clock-time
	 * (in GMT seconds) would be bumped up an hour for daylight savings
	 * time.
	 */
#if HAVE_TM_ISDST && !TIMEZONE_DECLARED && !HAVE_TM_GMTOFF
	if (tm.tm_isdst)	sec -= HOUR;
#endif
	return sec;
}

long	packdate (
	_ARX(int,	 year)
	_ARX(int,	 mon)
	_ARX(int,	 day)
	_ARX(int,	 hour)
	_ARX(int,	 min)
	_AR1(int,	 s)
		)
	_DCL(int,	 year)
	_DCL(int,	 mon)
	_DCL(int,	 day)
	_DCL(int,	 hour)
	_DCL(int,	 min)
	_DCL(int,	 s)
{
	auto	time_t	sec = s;
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

	return (sec + gmt_offset(sec));
}

/************************************************************************
 *	Test Driver							*
 ************************************************************************/
#ifdef	TEST
_MAIN
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
#if HAVE_TM_ZONE
	printf("  zone  =%s\n",  tm.tm_zone);
#endif
#if HAVE_TM_GMTOFF
	printf("  gmtoff=%ld\n", tm.tm_gmtoff);
#endif

	then = packdate (tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
			tm.tm_hour,       tm.tm_min,   tm.tm_sec);
	printf("Packed time: %s", ctime(&then));
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	/* TEST */
