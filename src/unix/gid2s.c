#ifndef	lint
static	char	sccs_id[] = "@(#)gid2s.c	1.5 88/08/09 11:58:45";
#endif	lint

/*
 * Title:	gid2s.c (gid/string converter)
 * Author:	T.E.Dickey
 * Created:	10 Nov 1987
 * Modified:
 *		28 Jul 1988, added "<none>" for apollo.
 *
 * Function:	Maintain a lookup table of gid names for fast access.
 *		For any given argument, return a pointer to string
 *		defining the name.
 */

#include	"ptypes.h"
#include	<stdio.h>
#include	<grp.h>
extern	char	*ltostr(),
		*stralloc(),
		*strcpy();

typedef	struct {
	int	t_gid;
	char	*t_name;
	} TABLE;

#define	Q(j)	(q+j)->
#define	def_doalloc	def_GID_TABLE	/* lint (gould) */
	def_DOALLOC(TABLE)

char *
gid2s(gid)
{
register struct group *p;
register int	j;
static   TABLE	*q;
static   char	bfr[10];
static	unsigned qmax = 0;

	if ((qmax == 0) && setgrent() >= 0) {
		while (p = getgrent()) {
		register char *s = p->gr_name;
			q = DOALLOC(q,TABLE,qmax+1);
			Q(qmax)t_gid  = p->gr_gid;
			Q(qmax)t_name = stralloc(s);
			qmax++;
		}
		(void)endgrent();
	}

	for (j = 0; j < qmax; j++) {
		if (Q(j)t_gid == gid)
			return(Q(j)t_name);
	}
#ifdef	apollo
	if (gid == -3)
		(void)strcpy(bfr, "<none>");
	else
#endif	apollo
	(void)ltostr(bfr, (long)gid, 0);
	return(bfr);
}
