/*
 * Title:	m2comp.c
 * Author:	T.E.Dickey (from MDIFF program)
 * Created:	05 Jul 1989
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		13 Nov 1992, to chunk the allocations of array cells, reducing
 *			     overhead.  Corrected logic that freed cells (had
 *			     freed some more than once).
 *
 * Function:	This program is derived from one presented in "A File
 *		Comparison Program", by Webb Miller and Eugene W. Myers
 *		published in SOFTWARE -- PRACTICE AND EXPERIENCE, Vol 15
 *		(11, 1025-1040 November 1983)
 */

#define	STR_PTYPES
#define	SCOMP	m2comp			/* name of this module */
#include "td_scomp.h"

MODULE_ID("$Id: m2comp.c,v 12.4 1993/10/29 17:35:25 tom Exp $")

#define	INSERT	1
#define	DELETE	2

typedef	unsigned short	Line;		/* should not need long for line # */

#define	EDIT	struct	edit
	EDIT {
	EDIT	*link;		/* previous edit command */
	Line	line1;		/* line number in file1 */
	Line	line2;		/* line number in file2 */
	char	op;		/* INSERT or DELETE */
};

#define	SAVE	struct	save
	SAVE {
	SAVE	*link;
	int	used;
	EDIT	edit_struct[1];
};

#ifdef	lint
#define	NEW(cast,n)	(((cast *)0)+(n))
#define	REF(v,n)	v[n*size/4]
#else	/* !lint */
#define	NEW(cast,n)	(cast *)doalloc((char *)0, (n) * sizeof(cast))
#define	REF(v,n)	(SCOMP_TYPE)(((n)*size)+((char *)v))
#endif	/* lint/!lint */

#define	MATCH(ref,tst)	((*match)(REF(V1,ref), REF(V2,tst)))

/******************************************************************************/

void	m2comp(
	_ARX(SCOMP_TYPE,	v1)	/* "old" vector to compare	*/
	_ARX(int,		n1)	/* ...corresponding length	*/
	_ARX(SCOMP_TYPE,	v2)	/* "new" vector to compare	*/
	_ARX(int,		n2)	/* ...corresponding length	*/
	_ARX(int,		size)	/* size of vector-entry		*/
	_FNX(int,		match,	(SCOMP_MATCH_ARGS(p1,p2)))
	_FN1(void,		report,	(SCOMP_REPORT_ARGS(p1,p2)))
		)
	_DCL(SCOMP_TYPE,	v1)
	_DCL(SCOMP_TYPE,	v2)
	_DCL(int,		n1)
	_DCL(int,		n2)
	_DCL(int,		size)
	_DCL(int,		(*match)())
	_DCL(void,		(*report)())
{
	auto	char	*V1	= (char *)v1;
	auto	char	*V2	= (char *)v2;
	auto	int	done	= FALSE;
	auto	EDIT	*ep, *behind, *ahead, *a, *b, *x, *w;
	auto	int	change;
	auto	EDIT	*start	= 0;
	auto	unsigned max_d;		/* bound on size of edit script */
	auto	Line	ORIGIN,
			lower,		/* left-most diagonal under test */
			upper,		/* right-most diagonal under test */
			d,		/* current diagonal distance */
			row,		/* row number */
			col;		/* column number */
	register Line	k;		/* current diagonal */

	/* for each diagonal, two items are saved: */
	auto	Line	*last_d;	/* the row containing the last d */
	auto	EDIT	**script;	/* corresponding edit script */
	auto	EDIT	*new;
	register SAVE	*ptr;

	auto	SAVE	*save_list;	/* global, so we can free cells */
	auto	int	save_size;	/* # we get per allocation */
	auto	unsigned save_SIZE;	/* actual allocation-size */

	save_list = 0;
	save_size = (n1 + n2 + 1) / 2;
	save_SIZE = sizeof(SAVE) + (save_size-1) * sizeof(EDIT);

	ORIGIN	= (n1 > n2) ? n1 : n2;
	max_d	= 2 * ORIGIN;
	last_d	= NEW(Line, max_d + 1);
	script  = NEW(EDIT*,max_d + 1);

	/* initialize: 0 entries in D indicate identical prefixes */
	for (row = 0;
		row < n1 && row < n2 && MATCH(row,row);
			++row)
		;

	last_d[ORIGIN] = row;
	script[ORIGIN] = NULL;

	lower = (row == n1) ? ORIGIN + 1 : ORIGIN - 1;
	upper = (row == n2) ? ORIGIN - 1 : ORIGIN + 1;

	if (lower > upper) {
		dofree((char *)last_d);
		dofree((char *)script);
		return;
	}

	/*
	 * Loop over each value of the edit-distance.  Unlike miller/myers
	 * published algorithm, we do an exhaustive search, so we never should
	 * exceed 'max_d' (unless we bomb out due to lack of memory).
	 */
	for (d = 1; d <= max_d; ++d) {
		/* for each relevant diagonal */
		for (k = lower; k <= upper; k += 2) {

			/* get space for the next edit instruction */
			if ((ptr = save_list) == 0
			 || (ptr->used >= save_size)) {
				ptr = (SAVE *)doalloc((char *)0, save_SIZE);
				ptr->link = save_list;
				ptr->used = 0;
				save_list = ptr;
			}
			new = &(ptr->edit_struct[ptr->used++]);

			/* find a d on diagonal k */
			if (	k             == ORIGIN - d
			    || (k             != ORIGIN + d
			    &&  last_d[k + 1] >= last_d[k - 1])) {
				/*
				 * Moving down from the last d-1 on diagonal k+1
				 * puts you farther along diagonal k than does
				 * moving right from the last d-1 on diagonal
				 * k-1.
				 */
				row       = last_d[k + 1] + 1;
				new->link = script[k + 1];
				new->op   = DELETE;
			} else {
				/*
				 * Move right from the last d-1 on diagonal k-1
				 */
				row       = last_d[k - 1];
				new->link = script[k - 1];
				new->op   = INSERT;
			}

			/* Code common to the two cases */
			new->line1 = row;
			new->line2 = col = row + k - ORIGIN;
			script[k]  = new;

			/* Slide down the diagonal */
			while (row < n1
			    && col < n2
			    && MATCH(row,col)) {
				++row;
				++col;
			}

			last_d[k] = row;

			if (row == n1 && col == n2) {
				/* Hit southeast corner, have the answer */
				start = script[k];
				done  = TRUE;
				break;
			}

			if (row == n1)
				/* Hit last row, don't look to the left */
				lower = k + 2;

			if (col == n2)
				/* Hit last column, don't look to the right */
				upper = k - 2;
		}
		if (done)
			break;
		--lower;
		++upper;
	}
	if (!done) {
		failed("m2comp(exceed)");
		/*NOTREACHED*/
	}

	/*
	 * Report the differences
	 */

	/* un-reverse the script first */
	ahead = start;
	ep    = NULL;

	while (ahead != NULL) {
		behind   = ep;
		ep	 = ahead;
		ahead    = ahead->link;
		ep->link = behind;
	}

	while ((b = ep) != NULL) {
		if (ep->op == INSERT) {
			do {
				x = b;
				b = b->link;
			} while (b        != NULL
			    &&	 b->op	  == INSERT
			    &&	 b->line1 == ep->line1);

			(*report)(
				v1, ep->line1,  ep->line1-1,
				v2, ep->line2-1, x->line2-1);
		} else {
			do {
				a = b;
				b = b->link;
			} while (b        != NULL
			    &&	 b->op    == DELETE
			    &&	 b->line1 == a->line1 + 1);

			change = (b        != NULL
			    &&	  b->op    == INSERT
			    &&	  b->line1 == a->line1);

			if (change) {
				x = b;
				do {
					w = x;
					x = x->link;
				} while (x        != NULL
				    &&	 x->op    == INSERT
				    &&	 x->line1 == b->line1);

				(*report)(
					v1, ep->line1-1, a->line1-1,
					v2, ep->line2,   w->line2-1);
				b = x;
			} else {
				(*report)(
					v1, ep->line1-1,  a->line1-1,
					v2, ep->line2,   ep->line2-1);
			}
		}
		ep = b;
	}

	/*
	 * Finally, release temporary storage and return
	 */
	dofree((char *)last_d);
	dofree((char *)script);
	while (save_list) {
		register SAVE *p = save_list;
		save_list = p->link;
		dofree((char *)p);
	}
}

/************************************************************************
 *	test-driver: test_scomp.c					*
 ************************************************************************/
#ifdef	TEST
#include "test_cmp.c"
#endif
