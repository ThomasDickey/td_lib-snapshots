/*
 * Title:	strwcmp (compare wildcard-strings)
 * Author:	T.E.Dickey
 * Created:	27 Mar 1989
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	Performs a wildcard comparison between two strings a la unix
 *		filenames.
 */

#include "ptypes.h"

MODULE_ID("$Id: strwcmp.c,v 12.7 2010/07/05 16:07:46 tom Exp $")

#define	SINGLE	'?'
#define	MULTI	'*'

int
strwcmp(const char *wild, const char *name)
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
static void
default_test(void)
{
    static const char *names[] =
    {
	"", "a", "b", "ab", "ba", "aa", "bb", "aba"
    };
    static const char *wilds[] =
    {
	"",
	"*", "?", "a", "b",
	"?*", "*?", "??", "a*", "a?", "*a", "?a", "ab",
	"a*b", "ab*", "a?b", "ab?"
    };
    int j, k;

    PRINTF("\t");
    for (k = 0; k < (int) SIZEOF(names); k++)
	PRINTF("%-5s", names[k]);
    PRINTF("\n");

    for (j = 0; j < (int) SIZEOF(wilds); j++) {
	PRINTF("%s\t", wilds[j]);
	for (k = 0; k < (int) SIZEOF(names); k++)
	    PRINTF("%3d  ", strwcmp(wilds[j], names[k]));
	PRINTF("\n");
    }
}

_MAIN
{
    int j;
    if (argc > 1) {
	for (j = 1; j < argc; j++)
	    PRINTF("%s\t%d\n", argv[j], strwcmp(argv[1], argv[j]));
    } else
	default_test();
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif
