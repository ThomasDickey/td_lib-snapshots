#ifndef	lint
static	char	sccs_id[] = "@(#)uid2s.c	1.8 88/08/11 07:17:49";
#endif	lint

/*
 * Title:	uid2s.c (uid/string converter)
 * Author:	T.E.Dickey
 * Created:	10 Nov 1987
 * Modified:
 *		27 Jul 1988, use 'stralloc()', added special case for apollo
 *
 * Function:	Maintain a lookup table of uid names for fast access.
 *		For any given argument, return a pointer to string
 *		defining the name.
 */

#include	"ptypes.h"
#include	<pwd.h>
extern	char	*ltostr(),
		*stralloc(),
		*strcpy();

typedef	struct {
	int	t_uid;
	char	*t_name;
	} TABLE;

#define	Q(j)	(q+j)->
#define	def_doalloc	def_UID_TABLE	/* lint (gould) */
	/*ARGSUSED*/
	def_DOALLOC(TABLE)

char *
uid2s(uid)
{
extern	 struct passwd *getpwent();		/* cf: apollo sys5 */
extern		VOID	setpwent();
extern		VOID	endpwent();
register struct passwd *p;
register int	j;
static   TABLE	*q;
static   char	bfr[10];
static	unsigned qmax = 0;

	if (qmax == 0) {
		(void)setpwent();
		while (p = getpwent()) {
		register char *s = p->pw_name;
			q = DOALLOC(q, TABLE, qmax+1);
			Q(qmax)t_uid  = p->pw_uid;
			Q(qmax)t_name = stralloc(s);
			qmax++;
		}
		(void)endpwent();
	}

	for (j = 0; j < qmax; j++) {
		if (Q(j)t_uid == uid)
			return(Q(j)t_name);
	}
#ifdef	apollo
	if (uid == -3) {
		(void)strcpy(bfr, "<none>");
	} else
#endif	apollo
	(void)ltostr(bfr, (long)uid, 0);
	return(bfr);
}
