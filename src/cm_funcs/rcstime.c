#ifndef	lint
static	char	*Id = "$Id: rcstime.c,v 11.0 1992/02/07 15:03:08 ste_cm Rel $";
#endif

/*
 * Title:	rcstime.c
 * Author:	T.E.Dickey
 * Created:	05 Feb 1992
 *
 * Function:	converts to/from string/time_t values for RCS archives.
 */

#include "ptypes.h"
#include "rcsdefs.h"
#include <time.h>

/*
 * Decode an rcs archive-date
 */
time_t
rcs2time(
_AR1(char *,	from))
_DCL(char *,	from)
{
	time_t	the_time = 0;
	int	year, mon, day, hour, min, sec;

	newzone(5,0,FALSE);	/* format for EST5EDT */

	if (sscanf(from, FMT_DATE, &year, &mon, &day, &hour, &min, &sec) == 6)
		the_time = packdate(1900+year, mon, day, hour, min, sec);

	oldzone();
	return the_time;
}

/*
 * Convert a unix time to an rcs archive-date
 */
void
time2rcs(
_ARX(char *,	to)
_AR1(time_t,	from)
	)
_DCL(char *,	to)
_DCL(time_t,	from)
{
	struct	tm *t;

	newzone(5,0,FALSE);	/* format for EST5EDT */
	t = localtime(&from);
	FORMAT(to, FMT_DATE,
		t->tm_year, t->tm_mon + 1,
		t->tm_mday, t->tm_hour,
		t->tm_min,  t->tm_sec);
	oldzone();
}
