#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/cm_funcs/RCS/rcskeys.c,v 2.0 1988/09/28 09:31:03 ste_cm Exp $";
#endif	lint

/*
 * Title:	rcskeys.c (decode RCS keywords)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * $Log: rcskeys.c,v $
 * Revision 2.0  1988/09/28 09:31:03  ste_cm
 * BASELINE Thu Apr  6 09:45:13 EDT 1989
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
