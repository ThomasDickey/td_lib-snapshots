#ifndef	lint
static	char	Id[] = "$Id: rcskeys.c,v 9.0 1991/05/15 09:40:33 ste_cm Rel $";
#endif

/*
 * Title:	rcskeys.c (decode RCS keywords)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * $Log: rcskeys.c,v $
 * Revision 9.0  1991/05/15 09:40:33  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:40:33  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  89/04/20  12:36:00  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  89/04/20  12:36:00  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/04/20  12:36:00  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/04/20  12:36:00  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  89/04/20  12:36:00  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  89/04/20  12:36:00  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.1  89/04/20  12:36:00  dickey
 *		include "ptypes.h" before "rcsdefs.h" because of function-prototypes there.
 *		
 *		Revision 2.0  88/09/28  09:31:03  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.4  88/09/28  09:31:03  dickey
 *		sccs2rcs keywords
 *		
 *		28 Sep 1988, added 'strict' keyword.
 *		19 Aug 1988, added 'log', 'text' keywords so we can scan the
 *			     entire archive-file.
 *
 * Function:	Given a null-terminated string, parse it to see what type of
 *		RCS keyword it may be.  A <num> is legal here, since we may
 *		have a version-string which begins a <delta> section.
 *
 *		The indices of the array 'keys[]' are given as symbols in
 *		the "rcsdefs.h" file.
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	"rcsdefs.h"
#include	<ctype.h>


rcskeys(s)
char	*s;
{
static
char	*keys[] = {
				/* <admin> section			*/
		"head",		/*0:	head	{<num>};		*/
		"access",	/*1:	access	{<id>}*;		*/
		"symbols",	/*2:	symbols	{<id> : <num>}*;	*/
		"locks",	/*3:	locks	{<id> : <num>}*;	*/
		"comment",	/*4:	comment	{<string};		*/
		"strict",	/*5:	strict-locking in effect	*/
				/* <delta> section begins with <num>	*/
		"date",		/*6:	date		<num>;		*/
		"author",	/*7:	author		{<id>};		*/
		"state",	/*8:	state		{<id>};		*/
		"branches",	/*9:	branches	{<num>}*;	*/
		"next",		/*10:	next		{<num>};	*/
		"desc",		/*11: description ends header		*/
				/* <deltatext> begins with <num>	*/
		"log",		/*12: delta log-message			*/
		"text"		/*13: delta text/editing commands	*/
		};
register int j;
	if (*s) {
		if (isdigit(*s)) {
			while (isdigit(*s) || (*s == '.'))	s++;
			return (*s ? -1 : S_VERS);
		}
		for (j = 0; j < sizeof(keys)/sizeof(keys[0]); j++) {
			if (!strcmp(keys[j], s))
				return (j);
		}
		return (-2);	/* no match at all */
	}
	return (-1);
}
