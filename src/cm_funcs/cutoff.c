#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/cm_funcs/RCS/cutoff.c,v 8.0 1989/07/25 08:56:09 ste_cm Rel $";
#endif	lint

/*
 * Title:	cutoff.c (parse SCCS-style cutoff date)
 * Author:	T.E.Dickey
 * Created:	20 May 1988 (from 'sccsdate.c')
 * $Log: cutoff.c,v $
 * Revision 8.0  1989/07/25 08:56:09  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  89/07/25  08:56:09  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/07/25  08:56:09  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/07/25  08:56:09  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  89/07/25  08:56:09  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.1  89/07/25  08:56:09  dickey
 *		recompiled with apollo SR10 -- mods for function prototypes
 *		
 *		Revision 3.0  88/06/13  06:54:53  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/06/13  06:54:53  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.3  88/06/13  06:54:53  dickey
 *		sccs2rcs keywords
 *		
 *		13 Jun 1988, use 'newzone()'.
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
int	argc;
char	**argv;
{
time_t	date;
char	bfr[80],
	*d = strcpy(bfr, "991231235959"),
	*s = optarg;

	newzone(5,0,0);		/* interpret date in EST5EDT */

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
	date = packdate(1900+Z(0),Z(1),Z(2),Z(3),Z(4),Z(5));
	oldzone();		/* restore original timezone */
	return (date);
}
