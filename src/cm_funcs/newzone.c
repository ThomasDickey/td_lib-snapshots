#ifndef	lint
static	char	Id[] = "$Id: newzone.c,v 8.0 1989/10/04 11:58:57 ste_cm Rel $";
#endif	lint

/*
 * Title:	newzone.c (set new timezone)
 * Author:	T.E.Dickey
 * Created:	09 Jun 1988
 * $Log: newzone.c,v $
 * Revision 8.0  1989/10/04 11:58:57  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  89/10/04  11:58:57  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/10/04  11:58:57  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/10/04  11:58:57  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.1  89/10/04  11:58:57  dickey
 *		lint (apollo SR10.1)
 *		
 *		Revision 4.0  89/07/25  09:11:02  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.1  89/07/25  09:11:02  dickey
 *		recompiled with apollo SR10 -- mods for function prototypes
 *		
 *		Revision 3.0  88/09/13  12:47:57  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/09/13  12:47:57  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.9  88/09/13  12:47:57  dickey
 *		sccs2rcs keywords
 *		
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

#define	STR_PTYPES
#include	"ptypes.h"
#include	<stdio.h>
#include	<time.h>
extern	time_t	time();
extern	char	*getenv();
extern	char	*ctime();

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

#ifndef	SYSTEM5
#ifndef	apollo
typedef	char	**VEC;
extern	VEC	environ;
extern	char	*stralloc();
	/*ARGSUSED*/
	def_ALLOC(char *)
#endif	apollo
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
int	minutes;
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
reset_tz(name)
char	*name;
{
#ifdef	apollo
	putenv(name);
	tzset();
#else	apollo
#ifdef	SYSTEM5
	putenv(name);
	tzset();
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
#ifdef	GOULD_NP1
	tzset();
#endif	GOULD_NP1
#endif	SYSTEM5
#endif	apollo
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
int	hours, minutes, apres;
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
