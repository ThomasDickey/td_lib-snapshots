#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/string/RCS/strtrim.c,v 4.0 1989/03/08 13:25:13 ste_cm Rel $";
#endif	lint

/*
 * Title:	strtrim.c
 * Author:	T.E.Dickey
 * Created:	08 Mar 1989
 * $Log: strtrim.c,v $
 * Revision 4.0  1989/03/08 13:25:13  ste_cm
 * BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *
 *		Revision 3.0  89/03/08  13:25:13  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  89/03/08  13:25:13  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  89/03/08  13:25:13  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Trims trailing whitespace from a string.  Returns a pointer to
 *		the resulting string.
 */

#include	<ctype.h>

char	*strtrim(s)
char	*s;
{
	register char	*base = s;
	s += strlen(s);
	while (--s >= base) {
		if (isspace(*s))
			*s = '\0';
		else
			break;
	}
	return (base);
}
