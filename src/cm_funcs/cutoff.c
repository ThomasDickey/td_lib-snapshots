#ifndef	lint
static	char	Id[] = "$Id: cutoff.c,v 10.4 1992/06/30 07:46:25 dickey Exp $";
#endif

/*
 * Title:	cutoff.c (parse SCCS-style cutoff date)
 * Author:	T.E.Dickey
 * Created:	20 May 1988 (from 'sccsdate.c')
 * Modified:
 *		30 Jun 1992, 'optind' should always be index of most recent
 *			     argv-entry parsed.
 *		08 Jan 1992, allow year to be given as "xx", "19xx" or "20xx";
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		25 Jul 1989, recompiled with apollo SR10 -- mods for function
 *			     prototypes
 *		13 Jun 1988, use 'newzone()'.
 *
 * Function:	Convert an SCCS-style cutoff date-string to unix time.  The
 *		string is read from the argument list using 'getopt()'.
 */

#define	STR_PTYPES
#include	<ptypes.h>
#include	<ctype.h>
#include	<time.h>

#define	Z(n)	twod(&bfr[n+n])

static
twod (
_AR1(char *,	s))
_DCL(char *,	s)
{
	return (10*(s[0]-'0') + s[1]-'0');
}

time_t
cutoff (
_ARX(int,	argc)
_AR1(char **,	argv)
	)
_DCL(int,	argc)
_DCL(char **,	argv)
{
	int	first	= TRUE;
	int	year	= 1900;
	time_t	date;
	char	bfr[80],
		*d = strcpy(bfr, "991231235959"),
		*s = optarg;

	newzone(5,0,0);		/* interpret date in EST5EDT */

	/*
	 * Decode the date from the argument list
	 */
	while (*d) {
		if (isdigit(*s)) {
			*d++ = *s++;

			if (first && (d - bfr) == 2) {
				first = FALSE;
				if (!strncmp(bfr, "19", 2))
					d = bfr;
				else if (!strncmp(bfr, "20", 2)) {
					year = 2000;
					d = bfr;
				}
			}

		} else {
			if ((d-bfr) & 1) {
				*d = d[-1];
				d[-1] = '0';
				d++;
			}
			if (*s)
				s++;
			else {
				if (optind < argc) {
					if (isdigit(*argv[optind+1]))
						s = argv[++optind];
					else
						break;
				} else
					break;
			}
		}
	}

	date = packdate(year+Z(0),Z(1),Z(2),Z(3),Z(4),Z(5));
	oldzone();		/* restore original timezone */
	return (date);
}

#ifdef	TEST
_MAIN
{
	time_t	it;
	register int	j;
	while ((j = getopt(argc, argv, "c:")) != EOF)
		switch (j) {
		case 'c':
			it = cutoff(argc, argv);
			PRINTF("=>%s", ctime(&it));
			break;
		default:
			FPRINTF(stderr, "expecting -c option\n");
			exit(FAIL);
		}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
