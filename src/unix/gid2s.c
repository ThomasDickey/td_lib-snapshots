#ifndef	NO_SCCS_ID
static	char	sccs_id[] = "@(#)gid2s.c	1.2 88/05/17 10:05:13";
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
		*ltostr(),
		*strcpy();

typedef	struct {
	int	t_gid;
	char	*t_name;
	} TABLE;

#define	Q(j)	(q+j)->

#ifdef	lint
#define	DOALLOC(c,p,n)	(c *)0
#else	lint
#define	DOALLOC(c,p,n)	(c *)doalloc((char *)p, (n)*sizeof(c))
#endif	lint

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
			q = DOALLOC(TABLE,q,qmax+1);
			Q(qmax)t_gid  = p->gr_gid;
			Q(qmax)t_name = strcpy(DOALLOC(char,0,strlen(s)+1), s);
			qmax++;
		}
		(void)endgrent();
	}

	for (j = 0; j < qmax; j++) {
		if (Q(j)t_gid == gid)
			return(Q(j)t_name);
	}
	(void)ltostr(bfr, (long)gid, 0);
	return(bfr);
}
