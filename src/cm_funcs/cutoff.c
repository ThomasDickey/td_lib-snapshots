#ifndef	lint
static	char	sccs_id[] = "@(#)cutoff.c	1.1 88/05/20 09:47:20";
#endif	lint

/*
 * Title:	cutoff.c (parse SCCS-style cutoff date)
 * Author:	T.E.Dickey
 * Created:	20 May 1988 (from 'sccsdate.c')
 * Modified:
 *
 * Function:	Convert an SCCS-style cutoff date-string to unix time.  The
 *		string is read from the argument list using 'getopt()'.
 */

#include	"ptypes.h"

#include	<stdio.h>
#include	<ctype.h>
#include	<time.h>
extern	long	packdate();
extern	char	*strcpy();

extern	char	*optarg;	/* points to beginning of argument */
extern	int	optind;

#define	Z(n)	twod(&bfr[n+n])

static
twod (s)
char	*s;
{
	return (10*(s[0]-'0') + s[1]-'0');
}

time_t
cutoff (argc, argv)
char	*argv[];
{
char	bfr[80],
	*d = strcpy(bfr, "991231235959"),
	*s = optarg;

	/*
	 * Ensure that we will interpret the date in the form in which it
	 * is stored in the SCCS file.
	 */
	putenv("TZ=EST5EDT");
	putenv("TZNAME=EST,EDT");
#ifdef	SYSTEM5
	tzset();
#endif	SYSTEM5

	/*
	 * Decode the date from the argument list
	 */
	while (*d) {
		if (isdigit(*s))	*d++ = *s++;
		else {
			if ((d-bfr) & 1) {
				*d = d[-1];
				d[-1] = '0';
				d++;
			}
			if (*s)
				s++;
			else {
				if (optind < argc) {
					if (isdigit(*argv[optind]))
						s = argv[optind++];
					else
						break;
				} else
					break;
			}
		}
	}
	return (packdate(1900+Z(0),Z(1),Z(2),Z(3),Z(4),Z(5)));
}
