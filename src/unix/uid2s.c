#ifndef	lint
static	char	sccs_id[] = "@(#)uid2s.c	1.2 88/05/17 10:04:53";
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
		*ltostr(),
		*strcpy();

typedef	struct {
	int	t_uid;
	char	*t_name;
	} TABLE;

#define	Q(j)	(q+j)->

#ifdef	lint
#define	DOALLOC(c,p,n)	(c *)0
#else	lint
#define	DOALLOC(c,p,n)	(c *)doalloc((char *)p, (n)*sizeof(c))
#endif	lint

char *
uid2s(uid)
{
register struct passwd *p;
register int	j;
static   TABLE	*q;
static   char	bfr[10];
static	unsigned qmax = 0;

	if ((qmax == 0) && setpwent() >= 0) {
		while (p = getpwent()) {
		register char *s = p->pw_name;
			q = DOALLOC(TABLE,q, qmax+1);
			Q(qmax)t_uid  = p->pw_uid;
			Q(qmax)t_name = strcpy(DOALLOC(char,0,strlen(s)+1), s);
			qmax++;
		}
		(void)endpwent();
	}

	for (j = 0; j < qmax; j++) {
		if (Q(j)t_uid == uid)
			return(Q(j)t_name);
	}
	(void)ltostr(bfr, (long)uid, 0);
	return(bfr);
}
