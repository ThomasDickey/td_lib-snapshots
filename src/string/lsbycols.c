#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/string/RCS/lsbycols.c,v 5.0 1989/04/20 13:53:19 ste_cm Rel $";
#endif	lint

/*
 * Title:	list_by_cols.c
 * Author:	T.E.Dickey
 * Created:	17 Apr 1989
 *
 * Function:	Given an array of structures whose first member is a pointer
 *		to a character-string, print the array of strings in columns
 *		rather than by rows.
 *
 * Arguments:	listp	- pointer to the array of structures
 *		sizep	- size of a structure in the array
 *		num	- number of items in the array
 */

#include <stdio.h>
#include <string.h>

#ifdef	lint
#define	LIST(n)	(*(listp + n))		/* close enough to fool lint */
#else	!lint
#define	LIST(n)	(*(char **)((char *)listp + (sizep * n)))
#endif	lint/!lint
#define	MAXCOL	80

/*ARGSUSED*/
list_by_cols(listp, sizep, num)
char	**listp;
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
					printf("\n");
				break;
			}
			if (k != 0)
				printf("%*s", gap, " ");
			if ((this + rows >= num)
			||  (k == cols - 1)) {
				printf("%s\n", LIST(this));
				break;
			} else
				printf("%-*s", maxlen, LIST(this));
		}
	}
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
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
		list_by_cols(tbl, sizeof(tbl[0]), sizeof(tbl)/sizeof(tbl[0]));
	}
}
#endif	TEST
