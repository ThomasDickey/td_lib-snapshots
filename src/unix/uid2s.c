#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/unix/RCS/uid2s.c,v 3.0 1988/08/12 09:36:56 ste_cm Rel $";
#endif	lint

/*
 * Title:	uid2s.c (uid/string converter)
 * Author:	T.E.Dickey
 * Created:	10 Nov 1987
 * $Log: uid2s.c,v $
 * Revision 3.0  1988/08/12 09:36:56  ste_cm
 * BASELINE Mon Jun 19 13:27:01 EDT 1989
 *
 *		Revision 2.0  88/08/12  09:36:56  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.10  88/08/12  09:36:56  dickey
 *		sccs2rcs keywords
 *		
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
extern		V_OR_I	setpwent();
extern		V_OR_I	endpwent();
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
