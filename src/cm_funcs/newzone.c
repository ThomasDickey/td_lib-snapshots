#ifndef	lint
static	char	sccs_id[] = "@(#)newzone.c	1.6 88/08/09 09:33:26";
#endif	lint

/*
 * Title:	newzone.c (set new timezone)
 * Author:	T.E.Dickey
 * Created:	09 Jun 1988
 * Modified:
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
 *
 *		The 'putenv()' procedure would simplify some of this, but is not
 *		portable to antique unix systems.
 */

#include	"ptypes.h"
#include	<stdio.h>
#include	<time.h>
extern	time_t	time();
extern	char	*getenv();
extern	char	*ctime();
extern	char	*strcat();
extern	char	*strcpy();

#define	MINUTE	60
#define	HOUR	(60 * MINUTE)
#define	DAY	(24 * HOUR)
#define	SIX_MM	(6 * (30 * DAY))

#define	EST_REF	581961600	/* EST: jun 10, 1988 12:00 */
#define	NAMELEN	20		/* size of buffers for time-zone names */

#ifdef	TEST
static	time_t	now;
#endif	TEST

static	char	old_TZ[NAMELEN];
int	localzone;		/* public copy of minutes-west */

#if	!defined(SYSTEM5) && !defined(apollo)
typedef	char	**VEC;
extern	VEC	environ;
extern	char	*stralloc();
	def_ALLOC(char *)
#endif	SYSTEM5

/************************************************************************
 *	local procedures						*
 ************************************************************************/

/*
 * Given a number of minutes-west, return the string by which we will name
 * the resulting time-zone.
 */
static
char *
name_of_tz(minutes)
{
	register int	hours	= (minutes/60);
	static	 char	computed[NAMELEN];
	register char	*s = computed,
			*standard,
			*daylight;

	switch (hours) {
	case 5:	standard = "EST";	daylight = "EDT"; break;
	case 6:	standard = "CST";	daylight = "CDT"; break;
	case 7:	standard = "MST";	daylight = "MDT"; break;
	case 8:	standard = "PST";	daylight = "PDT"; break;
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
		daylight = "";
	}
	minutes %= 60;
	FORMAT(s, "%s%d", standard, hours);
	if (minutes)
		FORMAT(s + strlen(s), ":%02d", minutes);
	return (strcat(s, daylight));
}

/*
 * Set the time-zone environment with the specified string.
 */
static
reset_tz(name)
char	*name;
{
#if	defined(SYSTEM5) || defined(apollo)
	putenv(name);
#else	SYSTEM5
register unsigned j, k;
register char	*s;
int	len	= 3;		/* "TZ=" length */
int	found	= FALSE;	/* set if we find variable */
int	match	= FALSE;	/* true iff we need no change */

	for (j = 0; s = environ[j]; j++) {
		if (strlen(s) > len)
			if (!strncmp(s, name, len)) {
				found = TRUE;
				match = !strcmp(s,name);
				break;
			}
	}
	if (!match) {
		if (!found) {	/* allocate space for this in environ */
		VEC	newp = ALLOC(char *,j + 2);
			for (k = 0; k < j; k++)
				newp[k] = environ[k];
			newp[j+1] = 0;
			environ = newp;
		} else
			strfree(s);
		environ[j] = stralloc(name);
	}
#endif	SYSTEM5
#if	defined(apollo) || defined(GOULD_NP1) || defined(SYSTEM5)
	tzset();
#endif
}

/*
 * Initialize this module.  If the string 'old_TZ' has not been set, obtain
 * a reasonable value by asking for the time in effect for unix time origin
 * (0000 hours on 1 Jan 1970).
 */
static
init_tz()
{
	if (!*old_TZ) {
	time_t	clock = 0;
	struct	tm tm;

		tm = *localtime(&clock);
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
newzone(hours, minutes, apres)
{
char	new_TZ[NAMELEN];

	init_tz();
	minutes += (hours * 60);
	if (apres)
		minutes = -minutes;
	FORMAT(new_TZ, "TZ=%s", name_of_tz(minutes));
	reset_tz(new_TZ);
#ifdef	TEST
	printf("  newzone(%4d) = '%s'\n", minutes, new_TZ);
	printf("\t\t\t\t=>%s", ctime(&now));
#endif	TEST
}

/*
 * Restore the original timezone (from before invoking 'newzone()')
 */
oldzone()
{
	init_tz();
	reset_tz(old_TZ);
}

/************************************************************************
 *	test driver							*
 ************************************************************************/
#ifdef	TEST
test()
{
	newzone(5, 0, FALSE);
	newzone(8, 0, FALSE);
	newzone(4, 30, TRUE);
	newzone(4, 30, FALSE);
}

main()
{
	now = EST_REF;
	printf("** now = %s", ctime(&now));
	test();
	printf("** six-months ago\n");
	now -= SIX_MM;
	test();
}

failed(s)
char	*s;
{
	perror(s);
	exit(0);
	/*NOTREACHED*/
}
#endif	TEST
