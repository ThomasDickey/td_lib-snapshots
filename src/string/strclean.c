#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/string/RCS/strclean.c,v 4.0 1987/11/10 09:49:26 ste_cm Rel $";
#endif	lint

/*
 * Title:	strclean.c (string-cleaner)
 * Author:	T.E.Dickey
 * Created:	10 Nov 1987
 * $Log: strclean.c,v $
 * Revision 4.0  1987/11/10 09:49:26  ste_cm
 * BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
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
