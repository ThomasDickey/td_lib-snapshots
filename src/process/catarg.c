/*
 * Title:	catarg.c (concatenate argument)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		07 Dec 1989, lint (Apollo SR10.1)
 *		27 Feb 1989, treat any embedded blank as a pass-thru (even tabs)
 *
 * Function:	Concatenate strings from 'argv[]' onto a buffer which will
 *		later be split in 'bldarg[]'.  Unlike 'strcat()', however,
 *		we translate embedded blanks to another character so that
 *		they can be passed through via 'blarg()'.
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	<ctype.h>

MODULE_ID("$Id: catarg.c,v 12.5 2004/03/07 22:03:45 tom Exp $")

void
catarg(char *dst, char *src)
{
    dst += strlen(dst);
    if (*src) {
	(void) strcpy(dst, src);
	while (*dst) {
	    if (isspace(UCH(*dst)))
		*dst |= 0200;
	    dst++;
	}
	*dst++ = ' ';
	*dst = EOS;
    }
}

void
catarg2(char *dst, char *opt, char *arg)
{
    catarg(dst, opt);
    dst[strlen(dst) - 1] = EOS;
    catarg(dst, arg);
}
