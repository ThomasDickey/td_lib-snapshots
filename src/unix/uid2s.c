#ifndef	lint
static	char	sccs_id[] = "@(#)uid2s.c	1.1 87/11/10 09:49:26";
#endif	lint

/*
 * Title:	uid2s.c (uid/string converter)
 * Author:	T.E.Dickey
 * Created:	10 Nov 1987
 * Modified:
 *
 * Function:	Maintain a lookup table of uid names for fast access.
 *		For any given argument, return a pointer to string
 *		defining the name.
 */

#include	<stdio.h>
#include	<pwd.h>
extern	char	*doalloc(),
		*strcpy();

typedef	struct {
	int	t_uid;
	char	*t_name;
	} TABLE;

#define	Q(j)	(q+j)->

char *
uid2s(uid)
{
register struct passwd *p;
register int	j;
static   TABLE	*q;
static   char	bfr[10];
static	unsigned qmax = 0;

	if (!qmax) {
		setpwent();
		while (p = getpwent()) {
		register char *s = p->pw_name;
			q = (TABLE *)doalloc((char *)q, (qmax+1)*sizeof(TABLE));
			Q(qmax)t_uid  = p->pw_uid;
			Q(qmax)t_name = strcpy(doalloc((char *)0,(unsigned)strlen(s)+1), s);
			qmax++;
		}
		endpwent();
	}

	for (j = 0; j < qmax; j++) {
		if (Q(j)t_uid == uid)
			return(Q(j)t_name);
	}
	sprintf(bfr, "%d", uid);
	return(bfr);
}
