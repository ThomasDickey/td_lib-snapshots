#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: lsbycols.c,v 12.2 1993/10/29 17:35:25 dickey Exp $";
#endif

/*
 * Title:	list_by_cols.c
 * Author:	T.E.Dickey
 * Created:	17 Apr 1989
 * Modified:
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

#ifdef	lint
#define	LIST(n)	(*(listp + n))		/* close enough to fool lint */
#else	/* !lint */
#define	LIST(n)	(*(char **)((char *)listp + (sizep * n)))
#endif
#define	MAXCOL	80

/*ARGSUSED*/
void	list_by_cols(
	_ARX(char **,	listp)
	_ARX(int,	sizep)
	_AR1(int,	num)
		)
	_DCL(char **,	listp)
	_DCL(int,	sizep)
	_DCL(int,	num)
{
	register int	j, k;
	auto	 int	maxlen = 0,	/* length of widest column */
			this,
			gap,		/* gap between columns */
			cols,		/* number of columns to print */
			rows;		/* last row-number */

	for (j = 0; j < num; j++)
		if ((this = strlen(LIST(j))) > maxlen)
			maxlen = this;
	gap  = (maxlen / 4) + 3;
	cols = MAXCOL / (maxlen + gap);
	rows = (num + (cols - 1)) / cols;

	for (j = 0; j < rows; j++) {
		for (k = 0; k < cols; k++) {
			if ((this = j + (k * rows)) >= num) {
				if (k != 0)
					PRINTF("\n");
				break;
			}
			if (k != 0)
				PRINTF("%*s", gap, " ");
			if ((this + rows >= num)
			||  (k == cols - 1)) {
				PRINTF("%s\n", LIST(this));
				break;
			} else
				PRINTF("%-*s", maxlen, LIST(this));
		}
	}
}

#ifdef	TEST
_MAIN
{
	if (argc > 1)
		list_by_cols(argv+1, sizeof(argv[0]), argc-1);
	else {
		static	char	*tbl[] = {
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
	/*NOTREACHED*/
}
#endif
