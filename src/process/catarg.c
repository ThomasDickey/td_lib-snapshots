#ifndef	lint
static	char	Id[] = "$Id: catarg.c,v 10.0 1991/10/03 08:42:53 ste_cm Rel $";
#endif

/*
 * Title:	catarg.c (concatenate argument)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * Modified:
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

catarg(
_ARX(char *,	dst)
_AR1(char *,	src)
	)
_DCL(char *,	dst)
_DCL(char *,	src)
{
	dst += strlen(dst);
	if (*src) {
		(void)strcpy(dst, src);
		while (*dst) {
			if (isspace(*dst))
				*dst |= 0200;
			dst++;
		}
		*dst++ = ' ';
		*dst   = '\0';
	}
}
