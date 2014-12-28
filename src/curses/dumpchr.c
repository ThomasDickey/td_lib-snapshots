/*
 * Title:	dumpchr.c (dump character)
 * Author:	T.E.Dickey
 * Created:	20 Apr 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		
 *		Revision 8.1  91/05/15  10:01:57  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *
 * Function:	Write a printing form of the given character to a file.
 *
 */

#define CHR_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: dumpchr.c,v 12.6 2014/12/28 01:10:44 tom Exp $")

#define	OUT	FPRINTF(fp,

void
dumpchr(FILE *fp, int c)
{
    c &= 0377;
    if (!isascii(c)) {
	OUT "\\|");
	c = toascii(c);
    }
    if (isprint(c)) {
	if (c == '\\')
	    OUT "\\");
	else if (c == '"')
	    OUT "\\\"");
	else
	    OUT "%c", c);
    } else {
	switch (c) {
	case '\b':
	    OUT "\\b");
	    break;
	case '\n':
	    OUT "\\n");
	    break;
	case '\t':
	    OUT "\\t");
	    break;
	case '\f':
	    OUT "\\f");
	    break;
	case '\r':
	    OUT "\\r");
	    break;
	default:
	    OUT "\\%03o", c);
	}
    }
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
