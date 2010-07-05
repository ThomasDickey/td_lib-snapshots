/*
 * Title:	samebranch.c
 * Author:	T.E.Dickey
 * Created:	03 Aug 1992
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *
 * Function:	returns true iff the two revisions are on the same branch or
 *		both are on the trunk.
 */

#define STR_PTYPES
#include "ptypes.h"
#include "rcsdefs.h"

MODULE_ID("$Id: samebr.c,v 12.7 2010/07/05 11:13:04 tom Exp $")

static size_t
two_dots(const char *rev)
{
    const char *base = rev;
    int dots = 0;
    while (*rev) {
	if (*rev++ == '.')
	    if (++dots >= 2)
		break;
    }
    return (dots == 2) ? (size_t) (rev - base) : 0;
}

int
samebranch(const char *rev1, const char *rev2)
{
    while (*rev1 && *rev2) {
	size_t dots1 = two_dots(rev1), dots2 = two_dots(rev2);
	if (dots1 && dots2) {
	    if (dots1 == dots2
		&& !strncmp(rev1, rev2, dots1)) {
		rev1 += dots1;
		rev2 += dots2;
		continue;
	    } else
		break;
	} else if (!dots1 && !dots2)
	    return TRUE;
	else
	    break;
    }
    return FALSE;
}

#ifdef	TEST
static
void
do_test(const char *rev1, const char *rev2)
{
    PRINTF("%8s %8s => %d\n", rev1, rev2, samebranch(rev1, rev2));
}

_MAIN
{
    static const char *tbl[] =
    {
	"1",
	"21",
	"1.",
	"21.",
	"1.1",
	"21.1",
	"1.1.1",
	"1.21.1",
	"1.1.1.1",
	"1.1.21.1"
    };
    size_t j;
    size_t k;

    if (argc > 1) {
	for (j = 1; j < (size_t) argc; j++)
	    do_test(argv[1], argv[j]);
    } else {
	for (j = 0; j < SIZEOF(tbl); j++)
	    for (k = 0; k < SIZEOF(tbl); k++)
		do_test(tbl[j], tbl[k]);
    }
    exit(SUCCESS);
}
#endif
