/*
 * Title:	newzone.c (set new timezone)
 * Author:	T.E.Dickey
 * Created:	09 Jun 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		16 Feb 1996, 'new_TZ' has to be static for compat with 'putenv'
 *		29 Oct 1993, ifdef-ident
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
#define	TIM_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: newzone.c,v 12.10 2010/07/05 11:00:45 tom Exp $")

#define	SIX_MM	(6 * (30 * DAY))

#define	EST_REF	581961600	/* EST: jun 10, 1988 12:00 */
#define	NAMELEN	20		/* size of buffers for time-zone names */

#ifdef	TEST
static time_t now;
#endif

static char new_TZ[NAMELEN];
static char old_TZ[NAMELEN];
#if	!defined(LOCALZONE_DECLARED)
int localzone;			/* public copy of minutes-west */
#endif

/************************************************************************
 *	local procedures						*
 ************************************************************************/

/*
 * Given a number of minutes-west, return the string by which we will name
 * the resulting time-zone.
 */
static char *
name_of_tz(int minutes)
{
    int hours = (minutes / 60);
    static char computed[NAMELEN];
    char *s = computed;
    const char *standard, *DayLight;

    switch (hours) {
    case 5:
	standard = "EST";
	DayLight = "EDT";
	break;
    case 6:
	standard = "CST";
	DayLight = "CDT";
	break;
    case 7:
	standard = "MST";
	DayLight = "MDT";
	break;
    case 8:
	standard = "PST";
	DayLight = "PDT";
	break;
    default:
	hours = -hours;
	if (minutes < 0) {
	    standard = "GMT+";
	    minutes = -minutes;
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
static void
reset_tz(char *name)
{
    (void) putenv(name);
    tzset();
}

/*
 * Initialize this module.  If the string 'old_TZ' has not been set, obtain
 * a reasonable value by asking for the time in effect for unix time origin
 * (0000 hours on 1 Jan 1970).
 */
static void
init_tz(void)
{
    if (!*old_TZ) {
	time_t zero = 0;
	struct tm tm;

	tm = *localtime(&zero);
	if (tm.tm_hour >= 12) {
	    localzone = (24 - tm.tm_hour) * 60 - tm.tm_min;
	} else {
	    localzone = -((tm.tm_hour * 60) + tm.tm_min);
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
void
newzone(int hours, int minutes, int apres)
{
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
void
oldzone(void)
{
    init_tz();
    reset_tz(old_TZ);
}

/************************************************************************
 *	test driver							*
 ************************************************************************/
#ifdef	TEST
static void
test(void)
{
    newzone(5, 0, FALSE);
    newzone(8, 0, FALSE);
    newzone(4, 30, TRUE);
    newzone(4, 30, FALSE);
}

_MAIN
{
    (void) argc;
    (void) argv;

    now = EST_REF;
    PRINTF("** now = %s", ctime(&now));
    test();
    PRINTF("** six-months ago\n");
    now -= SIX_MM;
    test();
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif
