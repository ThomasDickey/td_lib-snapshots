#ifndef	lint
static	char	*Id = "$Id: strwcmp.c,v 12.1 1993/09/21 18:54:03 dickey Exp $";
#endif

/*
 * Title:	strwcmp (compare wildcard-strings)
 * Author:	T.E.Dickey
 * Created:	27 Mar 1989
 * Modified:
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	Performs a wildcard comparison between two strings a la unix
 *		filenames.
 */

#include "ptypes.h"

#define	SINGLE	'?'
#define	MULTI	'*'

int	strwcmp(
	_ARX(char *,	wild)
	_AR1(char *,	name)
		)
	_DCL(char *,	wild)
	_DCL(char *,	name)
{
	while (*wild || *name) {
		if (*wild == MULTI) {
			while (*wild == MULTI)
				wild++;
			while (*name) {
				if (!strwcmp(wild, name))
					return (0);
				name++;
			}
			break;
		} else if (*wild != *name) {
			if (*wild != SINGLE || *name == EOS)
				break;
		}
		wild++;
		name++;
	}
	return (*wild - *name);
}

#ifdef	TEST
void	default_test(_AR0)
{
	static	char	*names[] = {
		"",	"a",	"b",	"ab",	"ba",	"aa",	"bb",	"aba"
		};
	static	char	*wilds[] = {
		"",
		"*",	"?",	"a",	"b",
		"?*",	"*?",	"??",	"a*",	"a?",	"*a",	"?a",	"ab",
		"a*b",	"ab*",	"a?b",	"ab?"
		};
	register int	j,k;

	PRINTF("\t");
	for (k = 0; k < SIZEOF(names); k++)
		PRINTF("%-5s", names[k]);
	PRINTF("\n");

	for (j = 0; j < SIZEOF(wilds); j++) {
		PRINTF("%s\t", wilds[j]);
		for (k = 0; k < SIZEOF(names); k++)
			PRINTF("%3d  ", strwcmp(wilds[j], names[k]));
		PRINTF("\n");
	}
}

_MAIN
{
	register int	j;
	if (argc > 1) {
		for (j = 1; j < argc; j++)
			PRINTF("%s\t%d\n", argv[j], strwcmp(argv[1], argv[j]));
	} else
		default_test();
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
