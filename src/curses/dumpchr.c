#ifndef	lint
static	char	Id[] = "$Id: dumpchr.c,v 9.0 1991/05/15 10:01:57 ste_cm Rel $";
#endif

/*
 * Title:	dumpchr.c (dump character)
 * Author:	T.E.Dickey
 * Created:	20 Apr 1988
 * $Log: dumpchr.c,v $
 * Revision 9.0  1991/05/15 10:01:57  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  10:01:57  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  88/04/20  09:46:59  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  88/04/20  09:46:59  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/04/20  09:46:59  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/04/20  09:46:59  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/04/20  09:46:59  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/04/20  09:46:59  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/04/20  09:46:59  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.2  88/04/20  09:46:59  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Write a printing form of the given character to a file.
 *
 */

#include	<stdio.h>
#include	<ctype.h>

#define	OUT	(void)fprintf(fp,

dumpchr(fp,c)
FILE	*fp;
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
