#ifndef	lint
static	char	*Id = "$Id: strwcmp.c,v 9.0 1990/03/27 13:22:40 ste_cm Rel $";
#endif

/*
 * Title:	strwcmp (compare wildcard-strings)
 * Author:	T.E.Dickey
 * Created:	27 Mar 1989
 *
 * Function:	Performs a wildcard comparison between two strings a la unix
 *		filenames.
 */
#include "ptypes.h"

#define	SINGLE	'?'
#define	MULTI	'*'

strwcmp(wild, name)
char	*wild, *name;
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
#define	SIZEOF(v)	(sizeof(v)/sizeof(v[0]))
default_test()
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

	printf("\t");
	for (k = 0; k < SIZEOF(names); k++)
		printf("%-5s", names[k]);
	printf("\n");

	for (j = 0; j < SIZEOF(wilds); j++) {
		printf("%s\t", wilds[j]);
		for (k = 0; k < SIZEOF(names); k++)
			printf("%3d  ", strwcmp(wilds[j], names[k]));
		printf("\n");
	}
}

main(argc, argv)
char	*argv[];
{
	register int	j;
	if (argc > 1) {
		for (j = 1; j < argc; j++)
			printf("%s\t%d\n", argv[j], strwcmp(argv[1], argv[j]));
	} else
		default_test();
}
#endif
