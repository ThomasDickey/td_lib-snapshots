/*
 * Title:	name2s.c (convert name to external string)
 * Author:	T.E.Dickey
 * Created:	18 Aug 1988 (from ded2s.c)
 * Modified:
 *		11 Dec 2019, remove apollo translation-feature.
 *		01 Dec 2019, use locale-based validity checks for escaping
 *		07 Mar 2004, remove K&R support, indent'd.
 *		05 Nov 1995, added tilde to shell-characters
 *		21 Sep 1993, gcc-warnings
 *		04 Oct 1991, conversion to ANSI
 *		22 Jul 1991, allow space as a printing char.
 *		19 Aug 1988, added 'opt & 4' mode so we can make 'copy' work on
 *			     things like "/bin/[".
 *
 * Function:	Convert a filename-string to printing form (for display), or
 *		to form useful for shell commands.
 *
 * Arguments:	bfr	= buffer in which to place converted name.
 *		len	= maximum length of 'bfr[]'.
 *		name	= name to convert
 *		opt	= options, by bit:
 *			1=escapes,
 *			2=unused (was Apollo feature)
 *			4=underlying escapes (no unix escapes)
 *
 * Returns:	The length of the converted name.
 */

#define	CHR_PTYPES
#define	CUR_PTYPES
#define	STR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: name2s.c,v 12.12 2019/12/12 00:29:17 tom Exp $")

#define	isshell(c)	(strchr("*%?$()[]{}|<>^&;#\\\"`'~", c) != 0)

int
name2s(char *bfr, int len, const char *name, int opt)
{
    int num = 0;
    int c;
    int esc = opt & 1;

    while (((c = *name++) != EOS) && (len-- > 0)) {
	if (esc) {
	    if (isascii(c)) {
		if (iscntrl(c)
		    || isspace(c)
		    || isshell(c)) {
		    bfr[num++] = '\\';	/* escape the nasty thing */
		}
	    } else {
		if (!valid_shell_char(c))
		    bfr[num++] = '\\';	/* escaping may help here... */
	    }
	    bfr[num++] = (char) c;
	} else {
	    if (isascii(c)) {
		if (isprint(c)) {
		    bfr[num++] = (char) c;
		} else {
		    bfr[num++] = '?';
		}
	    } else {
		if (valid_curses_char(c)) {
		    bfr[num++] = (char) c;
		} else {
		    bfr[num++] = '?';
		}
	    }
	}
    }
    bfr[num] = EOS;
    return num;
}

#ifdef	TEST
/*ARGSUSED*/
_MAIN
{
    int j;
    int opt = 0;
    char bfr[BUFSIZ];

    while ((j = getopt(argc, argv, "eu")) != EOF)
	switch (j) {
	case 'e':
	    opt |= 1;
	    break;
	case 'u':
	    opt |= 2;
	    break;
	}
    for (j = optind; j < argc; j++) {
	name2s(bfr, sizeof(bfr), argv[j], opt);
	printf("%d:\t\"%s\" => \"%s\"\n", j - optind + 1, argv[j], bfr);
    }
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif /* TEST */
