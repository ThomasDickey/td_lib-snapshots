/*
 * Title:	packdate.c (pack numbers to make a UNIX date)
 * Author:	T.E.Dickey
 * Created:	26 Mar 1986
 * Modified:
 *		03 Jul 2002, make the mktime() changes work on Solaris.
 *		21 Apr 2002, if mktime() is available, use that.  For instance
 *			     cygwin uses 1974 for zero-time.
 *		16 Apr 2002, reorder ifdef's, preferring tm_gmtoff (BSD) over
 *			     timezone, to work with older Linux's.
 *		11 Jul 2001, undef timezone, which is (mis)defined in U/Win's
 *			     time.h for some macro
 *		31 Dec 1999, move "1900" adjustment here.
 *		21 Aug 1998, Solaris 'daylight' value records only current
 *			     time, not (as documented) a side-effect of
 *			     localtime.  Use tm.tm_isdst instead.
 *		08 Apr 1996, IRIX 'timezone' doesn't include daylight-savings,
 *			     use 'daylight' symbol.
 *		14 Oct 1995, not all systems have 'gettimeofday()', and some
 *			     that do (e.g., Solaris) do not implement the
 *			     timezone parameter.  Use 'timezone' symbol for
 *			     those.
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

MODULE_ID("$Id: packdate.c,v 12.21 2002/07/03 17:28:10 tom Exp $")

#define	LEAP(y)	(!(y&3))

#ifdef timezone
#undef timezone		/* workaround for U/Win bug */
#endif

long	gmt_offset(
	_AR1(time_t,	t))
	_DCL(time_t,	t)
{
	struct	tm tm;
	long	result	= 0;

	/*
	 * Parse the computed time using 'localtime' to avoid the complication
	 * of determining the range between the last-Sunday-in-April to the
	 * last-Sunday-in-October. Invoke it here so we can bypass the SunOs
	 * 'gettimeofday()' procedure, which does not work well.
	 */
	tm = *localtime(&t);

#if HAVE_TM_GMTOFF
	result -= tm.tm_gmtoff;
#else /* UNIX */
#if TIMEZONE_DECLARED
	result += timezone;
#else
#if HAVE_GETTIMEOFDAY
	{
	struct	timeval	t2;
	struct	timezone tz;
		(void)gettimeofday(&t2, &tz);
		result += (tz.tz_minuteswest * MINUTE);
	}
#else
#endif
#endif	/* TIMEZONE_DECLARED */

	/*
	 * Check to see if the local-time display for a given clock-time
	 * (in GMT seconds) would be bumped up an hour for daylight savings
	 * time.
	 */
#if HAVE_TM_ISDST
	if (tm.tm_isdst)	result -= HOUR;
#endif
#endif	/* HAVE_TM_GMTOFF */
	return result;
}

long	packdate (
	_ARX(int,	 year)
	_ARX(int,	 mon)
	_ARX(int,	 day)
	_ARX(int,	 hour)
	_ARX(int,	 min)
	_AR1(int,	 sec)
		)
	_DCL(int,	 year)
	_DCL(int,	 mon)
	_DCL(int,	 day)
	_DCL(int,	 hour)
	_DCL(int,	 min)
	_DCL(int,	 sec)
{
#if HAVE_MKTIME
	time_t result;
	struct tm tm;

	memset(&tm, 0, sizeof(tm));
	if (year > 1900)
		year -= 1900;
	tm.tm_year = year;
	tm.tm_mon = mon - 1;
	tm.tm_mday = day;
	tm.tm_hour = hour;
	tm.tm_min = min;
	tm.tm_sec = sec;
	tm.tm_isdst = -1;

	result = mktime(&tm);
#else
	auto	time_t	result = sec;
	register int	j;
static	int	m[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
/*			  jan,feb,mar,apr,may,jun,jul,aug,sep,oct,nov,dec */

	if (year < 200)
		year += 1900;

	result += (MINUTE * min) + (HOUR * hour);
	for (j = 1970; j < year; j++) {
		result += YEAR;
		if (LEAP(j))	result += DAY;
	}
	for (j = 1; j < mon; j++) {
		result += DAY * m[j-1];
	}
	if (mon > 2 && LEAP(year))	result += DAY;
	result += (day-1) * DAY;
	result += gmt_offset(result);
#endif
	return result;
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
#if HAVE_TM_ISDST
	printf("  isdst =%d\n",  tm.tm_isdst);
#endif
#if HAVE_TM_ZONE
	printf("  zone  =%s\n",  tm.tm_zone);
#endif
#if HAVE_TM_GMTOFF
	printf("  gmtoff=%ld\n", tm.tm_gmtoff);
#endif
#if HAVE_TIMEZONE
	printf("timezone=%ld\n", timezone);
#endif

	then = packdate (tm.tm_year, tm.tm_mon+1, tm.tm_mday,
			tm.tm_hour,  tm.tm_min,   tm.tm_sec);
	printf("Packed time: %s", ctime(&then));
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	/* TEST */
