#ifndef	lint
static	char	Id[] = "$Id: strclean.c,v 12.0 1991/10/04 07:59:55 ste_cm Rel $";
#endif

/*
 * Title:	strclean.c (string-cleaner)
 * Author:	T.E.Dickey
 * Created:	10 Nov 1987
 * Modified:
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

strclean(
_AR1(char *,	string))
_DCL(char *,	string)
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
