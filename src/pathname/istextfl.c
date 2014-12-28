/*
 * Title:	istextfile.c (is this a text-file?)
 * Author:	T.E.Dickey
 * Created:	24 Oct 1989
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	Test a given file to see if if contains characters which are
 *		not normally considered printable ASCII.  If so (or if the
 *		file cannot be opened), return false.
 *
 *		If the file is empty, true is returned.
 */

#define CHR_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: istextfl.c,v 12.7 2014/12/28 01:10:51 tom Exp $")

int
istextfile(const char *name)
{
    FILE *fp;
    char bfr[BUFSIZ];
    size_t n;
    int c;

    if ((fp = fopen(name, "r")) != 0) {
	while ((n = fread(bfr, sizeof(char), sizeof(bfr), fp)) > 0) {
	    while (n-- != 0) {
		c = bfr[n];
		if (!isascii(c)
		    || (!isprint(c) && !isspace(c))) {
		    FCLOSE(fp);
		    return (FALSE);
		}
	    }
	}
	FCLOSE(fp);
	return (TRUE);
    }
    return (FALSE);
}

/******************************************************************************/
#ifdef	TEST
_MAIN
{
    (void) argc;
    (void) argv;
    exit(EXIT_FAILURE);
    /*NOTREACHED */
}
#endif /* TEST */
