#ifndef	lint
static	char	Id[] = "$Id: newzone.c,v 12.1 1993/09/21 18:54:04 dickey Exp $";
#endif

/*
 * Title:	newzone.c (set new timezone)
 * Author:	T.E.Dickey
 * Created:	09 Jun 1988
 * Modified:
 *		21 Sep 1993, gcc-warnings
 *		24 Nov 1992, local copy of 'putenv()' is obsolete
 *		04 Oct 1991, conversion to ANSI
 *		09 Sep 1991, lint (apollo SR10.3)
 *		04 Oct 1989, lint (apollo SR10.1)
 *		25 Jul 1989, recompiled with apollo SR10 -- mods for function
 *			     prototypes
 *		28 Jul 1988, ifdef for two types of gould.
 *
 * Function:	This module contains two procedures, 'newzone()' and
 *		'oldzone()'. The 'newzone()' procedure sets the current
 *		process's notion of timezone so that 'ctime()' and 'packdate()'
 *		will perform appropriate conversions.  The 'oldzone()' procedure
 *		is used both to initialize this module, as well as to reset
 *		the time-zone if 'newzone()' is invoked.  The public variable
 *		'localzone' retains the value of minutes-west obtained when
 *		the module is initialized, and corresponds to the initial
 *		time-zone.
 *
 * Notes:	On Apollo SR9.7, the timezone does not get set properly.
 *		This does not affect the translation for 'ctime()' or
 *		'localtime()', but gives strange results for the minutes-west
 *		value returned by 'gettimeofday()' because this uses the
 *		underlying value of timezone set by the AEGIS 'tz' command
 *		which is global to all processes.
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	<time.h>

#define	MINUTE	60
#define	HOUR	(60 * MINUTE)
#define	DAY	(24 * HOUR)
#define	SIX_MM	(6 * (30 * DAY))

#define	EST_REF	581961600	/* EST: jun 10, 1988 12:00 */
#define	NAMELEN	20		/* size of buffers for time-zone names */

#ifdef	TEST
static	time_t	now;
#endif

static	char	old_TZ[NAMELEN];
int	localzone;		/* public copy of minutes-west */

/************************************************************************
 *	local procedures						*
 ************************************************************************/

/*
 * Given a number of minutes-west, return the string by which we will name
 * the resulting time-zone.
 */
static
char *	name_of_tz(
	_AR1(int,	minutes))
	_DCL(int,	minutes)
{
	register int	hours	= (minutes/60);
	static	 char	computed[NAMELEN];
	register char	*s = computed,
			*standard,
			*DayLight;

	switch (hours) {
	case 5:	standard = "EST";	DayLight = "EDT"; break;
	case 6:	standard = "CST";	DayLight = "CDT"; break;
	case 7:	standard = "MST";	DayLight = "MDT"; break;
	case 8:	standard = "PST";	DayLight = "PDT"; break;
	default:
		hours    = - hours;
		if (minutes < 0) {
			standard = "GMT+";
			minutes  = -minutes;
		} else if (!hours) {
			standard = "GMT-";
		} else {
			standard = "GMT";
		}
		DayLight = "";
	}
	minutes %= 60;
	FORMAT(s, "%s%d", standard, hours);
	if (minutes)
		FORMAT(s + strlen(s), ":%02d", minutes);
	return (strcat(s, DayLight));
}

/*
 * Set the time-zone environment with the specified string.
 */
static
void	reset_tz(
	_AR1(char *,	name))
	_DCL(char *,	name)
{
	(void)putenv(name);
	tzset();
}

/*
 * Initialize this module.  If the string 'old_TZ' has not been set, obtain
 * a reasonable value by asking for the time in effect for unix time origin
 * (0000 hours on 1 Jan 1970).
 */
static
void	init_tz(_AR0)
{
	if (!*old_TZ) {
	time_t	zero = 0;
	struct	tm tm;

		tm = *localtime(&zero);
		if (tm.tm_hour >= 12) {
			localzone = (24 - tm.tm_hour) * 60 - tm.tm_min;
		} else {
			localzone = - ((tm.tm_hour * 60) + tm.tm_min);
		}
		FORMAT(old_TZ, "TZ=%s", name_of_tz(localzone));
		reset_tz(old_TZ);
	}
}

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

/*
 * Set our timezone to a specified value
 */
void	newzone(
	_ARX(int,	hours)
	_ARX(int,	minutes)
	_AR1(int,	apres)
		)
	_DCL(int,	hours)
	_DCL(int,	minutes)
	_DCL(int,	apres)
{
	char	new_TZ[NAMELEN];

	init_tz();
	minutes += (hours * 60);
	if (apres)
		minutes = -minutes;
	FORMAT(new_TZ, "TZ=%s", name_of_tz(minutes));
	reset_tz(new_TZ);
#ifdef	TEST
	PRINTF("  newzone(%4d) = '%s'\n", minutes, new_TZ);
	PRINTF("\t\t\t\t=>%s", ctime(&now));
#endif
}

/*
 * Restore the original timezone (from before invoking 'newzone()')
 */
void	oldzone(_AR0)
{
	init_tz();
	reset_tz(old_TZ);
}

/************************************************************************
 *	test driver							*
 ************************************************************************/
#ifdef	TEST
static
void	test(_AR0)
{
	newzone(5, 0, FALSE);
	newzone(8, 0, FALSE);
	newzone(4, 30, TRUE);
	newzone(4, 30, FALSE);
}

_MAIN
{
	now = EST_REF;
	PRINTF("** now = %s", ctime(&now));
	test();
	PRINTF("** six-months ago\n");
	now -= SIX_MM;
	test();
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
