#ifndef	lint
static	char	Id[] = "$Id: bldarg.c,v 9.0 1991/05/15 10:00:23 ste_cm Rel $";
#endif

/*
 * Title:	bldarg.c (build argv-array)
 * Created:	17 Dec 1985
 * $Log: bldarg.c,v $
 * Revision 9.0  1991/05/15 10:00:23  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  10:00:23  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  88/07/29  10:47:47  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  88/07/29  10:47:47  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/07/29  10:47:47  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/07/29  10:47:47  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/07/29  10:47:47  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/07/29  10:47:47  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/07/29  10:47:47  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.4  88/07/29  10:47:47  dickey
 *		sccs2rcs keywords
 *		
 *		29 Jul 1988, if the string has a trailing blank, don't put a
 *			     "" entry in vector!
 *		26 May 1988, accommodate changes in 'catarg()', which sets
 *			     spaces to non-ASCII characters to pass them thru
 *			     this procedure.
 *
 * Function:	Chop a command-string into words and point to the pieces via
 *		an argv-vector so that we can use the processed argument list
 *		in a call on 'execv' or 'execvp'.
 *
 *		We use this approach so that environment variables can include
 *		calling-options, e.g.,
 *
 *			setenv MORE "/usr/ucb/more -s"
 *
 * Arguments:	argc	- maximum length of 'argv[]'.
 *		argv[]	- array of pointers to char which is loaded here.
 *		string	- the command-string to parse.  It is destroyed, since
 *			  all spaces are turned to nulls.
 *
 * Bugs:	There is no provision for making the array 'argv[]' longer if
 *		we run out of room.
 *
 *		We do not process quotes ' or "
 */

#include	<ctype.h>
#define	blank(c)	(isascii(c) && isspace(c))

bldarg (argc, argv, string)
char	*argv[], *string;
{
register int  j  = 0;
register char *s = string;

	while (*s && (j < argc-1)) {
		while (blank(*s))	
			*s++ = '\0';
		argv[j++] = *s ? s : 0;
		while (*s && !blank(*s)) {
			*s = toascii(*s);
			s++;
		}
	}
	argv[j] = 0;
}
