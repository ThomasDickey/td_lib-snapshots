/*
 * Title:	argv2file.c
 * Author:	T.E.Dickey
 * Created:	18 May 1989
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	Write an argv-style text vector to a file, with the given mode.
 */

#define	STR_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: arg2file.c,v 12.6 2014/12/28 01:10:51 tom Exp $")

int
argv2file(char **vec,
	  char *name,
	  char *mode)
{
    FILE *fp;
    int count;

    if ((fp = fopen(name, mode)) != 0) {
	count = 0;
	while (*vec) {
	    count += (int) strlen(*vec);
	    FPRINTF(fp, "%s\n", *vec++);
	}
	FCLOSE(fp);
    } else
	count = -1;

    return (count);
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
