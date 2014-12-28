/*
 * Title:	strtrim.c
 * Author:	T.E.Dickey
 * Created:	08 Mar 1989
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		04 Oct 1989, lint (apollo SR10.1)
 *		
 *
 * Function:	Trims trailing whitespace from a string.  Returns a pointer to
 *		the resulting string.
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	<ctype.h>

MODULE_ID("$Id: strtrim.c,v 12.6 2014/12/28 01:11:07 tom Exp $")

char *
strtrim(char *s)
{
    char *base = s;
    s += strlen(s);
    while (--s >= base) {
	if (isspace(UCH(*s)))
	    *s = EOS;
	else
	    break;
    }
    return (base);
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
