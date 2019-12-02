/*
 * Title:	bldarg.c (build argv-array)
 * Created:	17 Dec 1985
 * Modified:
 *		01 Dec 2019, return resulting argc, for consistency.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
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

#define CHR_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: bldarg.c,v 12.9 2019/12/01 23:21:44 tom Exp $")

#define	blank(c)	(isascii(c) && isspace(c))

int
bldarg(int argc, char **argv, char *string)
{
    int j = 0;
    char *s = string;

    while (*s && (j < argc - 1)) {
	while (blank(UCH(*s)))
	    *s++ = EOS;
	argv[j++] = *s ? s : 0;
	while (*s && !blank(UCH(*s))) {
	    *s = (char) toascii(*s);
	    s++;
	}
    }
    argv[j] = 0;
    return j;
}

/******************************************************************************/
#ifdef	TEST
_MAIN
{
    (void) argc;
    (void) argv;
    exit(EXIT_FAILURE);
    /*NOTREACHED */
}
#endif /* TEST */
