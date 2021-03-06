/*
 * Title:	list_by_cols.c
 * Author:	T.E.Dickey
 * Created:	17 Apr 1989
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		30 May 1998, compile with g++
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	Given an array of structures whose first member is a pointer
 *		to a character-string, print the array of strings in columns
 *		rather than by rows.
 *
 * Arguments:	listp	- pointer to the array of structures
 *		sizep	- size of a structure in the array
 *		num	- number of items in the array
 */

#define	STR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: lsbycols.c,v 12.8 2014/07/22 13:47:34 tom Exp $")

#ifdef	lint
#define	LIST(n)	(*(listp + n))	/* close enough to fool lint */
#else /* !lint */
#define	LIST(n)	(*(char **)((void *)((char *)listp + (sizep * n))))
#endif
#define	MAXCOL	80

/*ARGSUSED*/
void
list_by_cols(const char **listp, int sizep, int num)
{
    int j, k;
    int maxlen = 0;		/* length of widest column */
    int len;
    int gap;			/* gap between columns */
    int cols;			/* number of columns to print */
    int rows;			/* last row-number */

    for (j = 0; j < num; j++)
	if ((len = (int) strlen(LIST(j))) > maxlen)
	    maxlen = len;
    gap = (maxlen / 4) + 3;
    cols = MAXCOL / (maxlen + gap);
    rows = (num + (cols - 1)) / cols;

    for (j = 0; j < rows; j++) {
	for (k = 0; k < cols; k++) {
	    if ((len = j + (k * rows)) >= num) {
		if (k != 0)
		    PRINTF("\n");
		break;
	    }
	    if (k != 0)
		PRINTF("%*s", gap, " ");
	    if ((len + rows >= num)
		|| (k == cols - 1)) {
		PRINTF("%s\n", LIST(len));
		break;
	    } else
		PRINTF("%-*s", maxlen, LIST(len));
	}
    }
}

#ifdef	TEST
_MAIN
{
    if (argc > 1) {
	list_by_cols((const char **) argv + 1, sizeof(argv[0]), argc - 1);
    } else {
	static const char *tbl[] =
	{
	    "a_potato",
	    "b_potato",
	    "c-xx",
	    "d_potato",
	    "e_potato",
	    "f-xxx",
	    "a_potato",
	    "b_potato",
	    "c-xx",
	    "d_potato",
	    "e_potato",
	    "f-xxx"
	};
	list_by_cols(tbl, sizeof(tbl[0]), SIZEOF(tbl));
    }
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif
