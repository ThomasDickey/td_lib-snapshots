#ifndef	lint
static	char	Id[] = "$Id: dumpchr.c,v 12.0 1991/10/03 16:07:36 ste_cm Rel $";
#endif

/*
 * Title:	dumpchr.c (dump character)
 * Author:	T.E.Dickey
 * Created:	20 Apr 1988
 * Modified:
 *		03 Oct 1991, converted to ANSI
 *		
 *		Revision 8.1  91/05/15  10:01:57  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *
 * Function:	Write a printing form of the given character to a file.
 *
 */

#include	"ptypes.h"
#include	<ctype.h>

#define	OUT	FPRINTF(fp,

dumpchr(
_ARX(FILE *,	fp)
_AR1(int,	c)
	)
_DCL(FILE *,	fp)
_DCL(int,	c)
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
		switch(c) {
		case '\b':	OUT "\\b");	break;
		case '\n':	OUT "\\n");	break;
		case '\t':	OUT "\\t");	break;
		case '\f':	OUT "\\f");	break;
		case '\r':	OUT "\\r");	break;
		default:	OUT "\\%03o", c);
		}
	}
}
