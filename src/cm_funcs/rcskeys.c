#ifndef	lint
static	char	Id[] = "$Id: rcskeys.c,v 12.1 1993/09/21 18:54:03 dickey Exp $";
#endif

/*
 * Title:	rcskeys.c (decode RCS keywords)
 * Author:	T.E.Dickey
 * Created:	26 May 1988
 * Modified:
 *		21 Sep 1993, gcc-warnings
 *		04 Oct 1991, conversion to ANSI
 *		06 Sep 1991, added debug-trace
 *		20 Apr 1989, include "ptypes.h" before "rcsdefs.h" because of
 *			     function-prototypes there.
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

int	rcskeys(
	_AR1(char *,	arg))
	_DCL(char *,	arg)
{
	static	struct	{
		int	code;
		char	*text;
		} keys[] = {
				/* <admin> section			*/
		{S_HEAD,	"head"},	/* {<num>};		*/
		{S_BRANCH,	"branch"},	/* {<num>}*;		*/
		{S_ACCESS,	"access"},	/* {<id>}*;		*/
		{S_SYMBOLS,	"symbols"},	/* {<id> : <num>}*;	*/
		{S_LOCKS,	"locks"},	/* {<id> : <num>}*;	*/
		{S_COMMENT,	"comment"},	/* {<string};		*/
		{S_STRICT,	"strict"},	/* strict-locking	*/
				/* <delta> section begins with <num>	*/
		{S_DATE,	"date"},	/* <num>;		*/
		{S_AUTHOR,	"author"},	/* {<id>};		*/
		{S_STATE,	"state"},	/* {<id>};		*/
		{S_BRANCHES,	"branches"},	/* {<num>}*;		*/
		{S_NEXT,	"next"},	/* {<num>};		*/
		{S_DESC,	"desc"},	/* ends header		*/
				/* <deltatext> begins with <num>	*/
		{S_LOG,		"log"},		/* log-message		*/
		{S_TEXT,	"text"}		/* text/editing commands*/
		};
	register int j;
	register char	*s = arg;
	int	code	= -1;

	if (*s) {
		if (isdigit(*s)) {
			while (isdigit(*s) || (*s == '.'))	s++;
			if (*s == EOS)
				code = S_VERS;
		} else {
			code = -2;	/* no match at all */
			for (j = 0; j < SIZEOF(keys); j++) {
				if (!strcmp(keys[j].text, s)) {
					code = keys[j].code;
					break;
				}
			}
		}
	}
	if (RCS_DEBUG > 1)
		PRINTF("++ rcskeys(%s) = %d\n", arg, code);
	return (code);
}
