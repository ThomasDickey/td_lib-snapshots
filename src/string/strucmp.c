/*
 * Title:	strucmp.c
 * Created:	05 Oct 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 * Function:	Compares two strings ignoring case.
 */

#define CHR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: strucmp.c,v 12.6 2010/07/05 16:07:32 tom Exp $")

int
strucmp(const char *a, const char *b)
{
    int x, y;

    for (;;) {
	if ((x = *a++) != (y = *b++)) {
	    if (x == 0 || y == 0)
		break;
	    UpperCase(x);
	    UpperCase(y);
	    if (x != y)
		break;
	} else if (x == 0)
	    break;
    }
    return (x - y);
}

#ifdef	TEST
_MAIN
{
    int j, k;
    static const char *tag[] =
    {"<", "==", ">"};

    for (j = 1; j < argc; j++) {
	k = strucmp(argv[1], argv[j]);
	if (k < 0)
	    k = -1;
	if (k > 0)
	    k = 1;
	PRINTF("%s %s %s\n", argv[1], tag[k + 1], argv[j]);
    }
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif
