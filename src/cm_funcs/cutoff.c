/*
 * Title:	cutoff.c (parse SCCS-style cutoff date)
 * Author:	T.E.Dickey
 * Created:	20 May 1988 (from 'sccsdate.c')
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		18 Jul 2000, Y2K fix
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		16 Jul 1992, 'optind' should always be index past most recent
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

MODULE_ID("$Id: cutoff.c,v 12.7 2010/07/10 00:08:21 tom Exp $")

#define	Z(n)	twod(&bfr[n+n])

static int
twod(char *s)
{
    return (10 * (s[0] - '0') + s[1] - '0');
}

time_t
cutoff(int argc, char **argv)
{
    int first = TRUE;
    int year = 1900;
    time_t date;
    char bfr[80], *d = strcpy(bfr, "991231235959"), *s = optarg;

    newzone(5, 0, 0);		/* interpret date in EST5EDT */

    /*
     * Decode the date from the argument list
     */
    while (*d) {
	if (isdigit(UCH(*s))) {
	    *d++ = *s++;

	    if (first && (d - bfr) == 2) {
		first = FALSE;
		if (!strncmp(bfr, "19", (size_t) 2))
		    d = bfr;
		else if (!strncmp(bfr, "20", (size_t) 2)) {
		    year = 2000;
		    d = bfr;
		} else if (Z(0) < 38) {
		    year = 100;
		}
	    }

	} else {
	    if ((d - bfr) & 1) {
		*d = d[-1];
		d[-1] = '0';
		d++;
	    }
	    if (*s)
		s++;
	    else {
		if (optind < argc) {
		    if (isdigit(UCH(*argv[optind])))
			s = argv[optind++];
		    else
			break;
		} else
		    break;
	    }
	}
    }

    date = packdate(year + Z(0), Z(1), Z(2), Z(3), Z(4), Z(5));
    oldzone();			/* restore original timezone */
    return (date);
}

#ifdef	TEST
_MAIN
{
    time_t it;
    int j;
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
    /*NOTREACHED */
}
#endif
