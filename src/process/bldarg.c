#ifndef	lint
static	char	sccs_id[] = "@(#)bldarg.c	1.1 85/12/17 11:59:12";
#endif	lint

/*
 * Created:	17 Dec 1985
 * Modified:	17 Dec 1985
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

bldarg (argc, argv, string)
char	*argv[], *string;
{
register int  j  = 0;
register char *s = string;

	while (*s && (j < argc-1)) {
		while (isspace(*s))		*s++ = '\0';
		argv[j++] = s;
		while (*s && !isspace(*s))	s++;
	}
	argv[j++] = 0;
}
