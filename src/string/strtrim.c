/*
 * Title:	strtrim.c
 * Author:	T.E.Dickey
 * Created:	08 Mar 1989
 * Modified:
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

MODULE_ID("$Id: strtrim.c,v 12.4 2002/07/03 13:07:33 tom Exp $")

char	*
strtrim(
_AR1(char *,	s))
_DCL(char *,	s)
{
	register char	*base = s;
	s += strlen(s);
	while (--s >= base) {
		if (isspace(UCH(*s)))
			*s = EOS;
		else
			break;
	}
	return (base);
}
