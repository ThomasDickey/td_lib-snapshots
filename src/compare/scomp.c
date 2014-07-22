/*
 * Title:	scomp.c (simple compare)
 * Author:	T.E.Dickey
 * Created:	24 May 1989
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		13 Nov 1992, use prototypes
 *
 * Function:	Compares two vectors (using user-supplied function) and reports
 *		differences (also via user-supplied function).  This is a
 *		simple-minded comparison, and and loses syncronization more
 *		easily than the standard comparison routines.
 *
 * Notes:	This is intended to correspond to a generic function a la
 *		'qsort()'.
 */

#define	SCOMP	scomp
#define STR_PTYPES
#include	"td_scomp.h"

MODULE_ID("$Id: scomp.c,v 12.6 2014/07/22 13:49:29 tom Exp $")

#define	REF(v,n)	(SCOMP_TYPE)((void *)(((n)*size)+((char *)v)))

#define	MATCH(ref,tst)	((*match)(REF(V1,ref), REF(V2,tst)))

#define	SYNC	3

void
SCOMP(SCOMP_TYPE v1,		/* "old" vector to compare  */
      int n1,			/* ...corresponding length  */
      SCOMP_TYPE v2,		/* "new" vector to compare  */
      int n2,			/* ...corresponding length  */
      int size,			/* size of vector-entry             */
      int (*match) (SCOMP_MATCH_ARGS(p1, p2)),
      void (*report) (SCOMP_REPORT_ARGS(p1, p2)))
{
    int x, y, z;
    int j1, j2;
    char *V1 = (char *) v1;
    char *V2 = (char *) v2;
    int diag, bottom, corner, ok;

    j1 = j2 = 0;

    while ((j1 < n1) && (j2 < n2)) {
	if (MATCH(j1, j2)) {
	    j1++, j2++;
	} else {
	    corner = (n1 - j1) + (n2 - j2);
	    ok = 0;
	    for (diag = 1; !ok && (diag < corner); diag++) {

		bottom = j1 + diag;
		if (bottom >= n1)
		    bottom = n1 - 1;

		for (y = bottom; y >= j1; y--) {
		    x = (j1 + diag) - y + j2;
		    if (x >= n2)
			break;
		    for (z = 0; z < SYNC; z++) {
			if ((y + z >= n1)
			    || (x + z >= n2)
			    || MATCH(y + z, x + z))
			    ok++;
		    }
		    if (ok == SYNC) {
			(*report) (v1, j1, y - 1, v2, j2, x - 1);
			j1 = y;
			j2 = x;
			break;
		    } else
			ok = 0;
		}
	    }
	    if (!ok)		/* remainder of file is diff */
		break;
	}
    }

    if (j1 < n1)		/* deletions on end ? */
	(*report) (v1, j1, n1 - 1, v2, n2, n2 - 1);
    if (j2 < n2)		/* insertions on end ? */
	(*report) (v1, n1, n1 - 1, v2, j2, n2 - 1);
}

/************************************************************************
 *	test-driver							*
 ************************************************************************/
#ifdef	TEST
#include "test_cmp.c"
#endif
