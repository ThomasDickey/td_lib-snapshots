/*
 * Title:	strclean.c (string-cleaner)
 * Author:	T.E.Dickey
 * Created:	10 Nov 1987
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		04 Oct 1989, lint (apollo SR10.1)
 *		
 *
 * Function:	"Clean" a string, removing all leading/repeated/trailing
 *		whitespace, as well as all nonprinting characters
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	<ctype.h>

MODULE_ID("$Id: strclean.c,v 12.5 2004/03/07 22:03:45 tom Exp $")

int
strclean(char *string)
{
    char *d, *s = string, *t;

    while (*s) {
	if (isspace(UCH(*s))) {
	    for (t = s; isspace(UCH(*t)); t++) ;
	    if (*t) {
		if (s != string)
		    *s++ = ' ';
		for (d = s; (*d++ = *t++) != EOS;) ;
	    } else
		*s = EOS;
	} else {
	    while (*s && !isspace(UCH(*s)))
		s++;
	}
    }
    return (strlen(string));
}
