#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: cmpqsort.c,v 12.1 1993/10/29 17:35:27 dickey Exp $";
#endif

/*
 * Title:	cmp_qsort.c (default comparison for qsort)
 * Author:	T.E.Dickey
 * Created:	18 Nov 1992, from 'walktree.c'
 * Modified:
 *		29 Oct 1993, ifdef-ident
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
