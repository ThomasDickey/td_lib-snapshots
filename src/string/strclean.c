#ifndef	lint
static	char	Id[] = "$Id: strclean.c,v 9.0 1991/05/15 09:55:19 ste_cm Rel $";
#endif

/*
 * Title:	strclean.c (string-cleaner)
 * Author:	T.E.Dickey
 * Created:	10 Nov 1987
 * $Log: strclean.c,v $
 * Revision 9.0  1991/05/15 09:55:19  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:55:19  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  89/10/04  12:54:22  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  89/10/04  12:54:22  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/10/04  12:54:22  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/10/04  12:54:22  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.1  89/10/04  12:54:22  dickey
 *		lint (apollo SR10.1)
 *		
 *		Revision 4.0  87/11/10  09:49:26  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  87/11/10  09:49:26  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  87/11/10  09:49:26  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  87/11/10  09:49:26  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	"Clean" a string, removing all leading/repeated/trailing
 *		whitespace, as well as all nonprinting characters
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	<ctype.h>

strclean(string)
char	*string;
{
register char	*d, *s = string, *t;

	while (*s) {
		if (isspace(*s)) {
			for (t = s; isspace(*t); t++);
			if (*t) {
				if (s != string) *s++ = ' ';
				for (d = s; *d++ = *t++;);
			} else
				*s = '\0';
		} else {
			while (*s && !isspace(*s))	s++;
		}
	}
	return(strlen(string));
}
