/*
 * Title:	rcstime.c
 * Author:	T.E.Dickey
 * Created:	05 Feb 1992
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		28 Apr 1993, added RCS_TIMEZONE environment variable to
 *			     simplify regression tests of rcshist.
 *		30 Oct 1992, RCS version 5 uses GMT, not local-time
 *
 * Function:	converts to/from string/time_t values for RCS archives.
 */

#define TIM_PTYPES
#include "ptypes.h"
#include "rcsdefs.h"

MODULE_ID("$Id: rcstime.c,v 12.4 1994/05/23 22:49:36 tom Exp $")

#if	RCS_VERSION >= 5
#define	RCS_ZONE(t)	gmt_offset(t)
#else
#define	RCS_ZONE(t)	0	/* no offset, use local-time */
#endif

/*
 * Decode an rcs archive-date
 */
time_t	rcs2time(
	_AR1(char *,	from))
	_DCL(char *,	from)
{
	time_t	the_time = 0;
	long	the_offset;
	int	year, mon, day, hour, min, sec;
	char	*s, *d;

	if (sscanf(from, FMT_DATE, &year, &mon, &day, &hour, &min, &sec) == 6)
		the_time = packdate(1900+year, mon, day, hour, min, sec);

	/* Allow override to make RCS 4 and RCS 5 files look the same.  This is
	 * really only for regression testing!
	 */
	if ((s = getenv("RCS_TIMEZONE")) != NULL)
		the_offset = strtol(s, &d, 0);
	else
		the_offset = RCS_ZONE(the_time);
	return the_time - the_offset;
}

/*
 * Convert a unix time to an rcs archive-date
 */
void	time2rcs(
	_ARX(char *,	to)
	_AR1(time_t,	from)
		)
	_DCL(char *,	to)
	_DCL(time_t,	from)
{
	struct	tm *t;

	from += RCS_ZONE(from);
	t = localtime(&from);
	FORMAT(to, FMT_DATE,
		t->tm_year, t->tm_mon + 1,
		t->tm_mday, t->tm_hour,
		t->tm_min,  t->tm_sec);
}
