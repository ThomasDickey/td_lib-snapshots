#ifndef	lint
static	char	Id[] = "$Id: catarg.c,v 9.0 1991/05/15 09:19:17 ste_cm Rel $";
#endif

/*
 * Title:	catarg.c (concatenate argument)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * $Log: catarg.c,v $
 * Revision 9.0  1991/05/15 09:19:17  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:19:17  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  89/12/07  15:18:51  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  89/12/07  15:18:51  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/12/07  15:18:51  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.1  89/12/07  15:18:51  dickey
 *		lint (Apollo SR10.1)
 *		
 *		Revision 5.0  89/02/27  11:13:47  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  89/02/27  11:13:47  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  89/02/27  11:13:47  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  89/02/27  11:13:47  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.3  89/02/27  11:13:47  dickey
 *		sccs2rcs keywords
 *		
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

catarg(dst, src)
char	*dst, *src;
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
