/*
 * Title:	strucpy.c (string-copy, uppercase)
 * Author:	T.E.Dickey
 * Created:	06 Oct 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		27 Jun 1990, added procedure 'strlcpy()' to this module. 
 *			     Altered 'strucpy()' to return its destination-arg
 *			     a la 'strcpy()'
 *		
 * Function:	Copy a string, converting its case (upper/lower). The source
 *		and destination arguments may be the same.
 */

#define CHR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: strucpy.c,v 12.5 2004/03/07 22:03:45 tom Exp $")

/*
 * Copy string, converting to uppercase
 */
char *
strucpy(char *dst, char *src)
{
    char *base = dst;
    int c;

    while ((c = *src++) != EOS) {
	UpperCase(c);
	*dst++ = c;
    }
    *dst = EOS;
    return (base);
}

/*
 * Copy string, converting to lowercase
 */
char *
strlwrcpy(char *dst, char *src)
{
    char *base = dst;
    int c;

    while ((c = *src++) != EOS) {
	LowerCase(c);
	*dst++ = c;
    }
    *dst = EOS;
    return (base);
}
