#ifndef	lint
static	char	sccs_id[] = "@(#)rcskeys.c	1.1 88/05/26 14:28:58";
#endif	lint

/*
 * Title:	rcskeys.c (decode RCS keywords)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * Modified:
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
		"locks",	/*3:	*/
		"comment",	/*4:	*/
				/* <delta> section begins with <num>	*/
		"date",		/*5:	date		<num>;		*/
		"author",	/*6:	author		{<id>};		*/
		"state",	/*7:	state		{<id>};		*/
		"branches",	/*8:	branches	{<num>}*;	*/
		"next",		/*9:	next		{<num>};	*/
		"desc"		/*10: description ends header */
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
