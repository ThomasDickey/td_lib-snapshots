#ifndef	NO_SCCS_ID
static	char	sccs_id[] = "@(#)gid2s.c	1.1 87/11/10 09:49:26";
#endif	NO_SCCS_ID

/*
 * Title:	gid2s.c (gid/string converter)
 * Author:	T.E.Dickey
 * Created:	10 Nov 1987
 * Modified:
 *
 * Function:	Maintain a lookup table of gid names for fast access.
 *		For any given argument, return a pointer to string
 *		defining the name.
 */

#include	<stdio.h>
#include	<grp.h>
extern	char	*doalloc(),
		*strcpy();

typedef	struct {
	int	t_gid;
	char	*t_name;
	} TABLE;

#define	Q(j)	(q+j)->

char *
gid2s(gid)
{
register struct group *p;
register int	j;
static   TABLE	*q;
static   char	bfr[10];
static	unsigned qmax = 0;

	if (!qmax) {
		setgrent();
		while (p = getgrent()) {
		register char *s = p->gr_name;
			q = (TABLE *)doalloc((char *)q, (qmax+1)*sizeof(TABLE));
			Q(qmax)t_gid  = p->gr_gid;
			Q(qmax)t_name = strcpy(doalloc((char *)0,(unsigned)strlen(s)+1), s);
			qmax++;
		}
		endgrent();
	}

	for (j = 0; j < qmax; j++) {
		if (Q(j)t_gid == gid)
			return(Q(j)t_name);
	}
	sprintf(bfr, "%d", gid);
	return(bfr);
}
