#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/process/RCS/catarg.c,v 5.0 1989/02/27 11:13:47 ste_cm Rel $";
#endif	lint

/*
 * Title:	catarg.c (concatenate argument)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * $Log: catarg.c,v $
 * Revision 5.0  1989/02/27 11:13:47  ste_cm
 * BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
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

#include	<ctype.h>
extern	char	*strcpy();

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
