#ifndef	lint
static	char	Id[] = "$Id: cmpqsort.c,v 12.0 1993/04/26 16:08:51 ste_cm Rel $";
#endif

/*
 * Title:	cmp_qsort.c (default comparison for qsort)
 * Author:	T.E.Dickey
 * Created:	18 Nov 1992, from 'walktree.c'
 * Modified:
 *		
 * Function:	Given a pathname, this successively invokes a user-supplied
 *
 * Arguments:	path	- the current working directory (ignored if level=0)
 *
 * Returns:	The number of items in the tree which both succeed with 'stat()'
 *		and with the user-specified access.
 */

#include	"td_qsort.h"

QSORT_FUNC(cmp_qsort)
{
	QSORT_CAST(q1,p1)
	QSORT_CAST(q2,p2)
	return (-strcmp(*p1, *p2));
}
