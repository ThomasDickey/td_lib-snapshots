#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/unix/RCS/gid2s.c,v 3.0 1988/08/12 09:36:30 ste_cm Rel $";
#endif	lint

/*
 * Title:	gid2s.c (gid/string converter)
 * Author:	T.E.Dickey
 * Created:	10 Nov 1987
 * $Log: gid2s.c,v $
 * Revision 3.0  1988/08/12 09:36:30  ste_cm
 * BASELINE Mon Jun 19 13:27:01 EDT 1989
 *
 *		Revision 2.0  88/08/12  09:36:30  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.8  88/08/12  09:36:30  dickey
 *		sccs2rcs keywords
 *		
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
	/*ARGSUSED*/
	def_DOALLOC(TABLE)

char *
gid2s(gid)
{
extern	 struct group  *getgrent();		/* cf: apollo sys5 */
extern		V_OR_I	setgrent();
extern		V_OR_I	endgrent();
register struct group *p;
register int	j;
static   TABLE	*q;
static   char	bfr[10];
static	unsigned qmax = 0;

	if (qmax == 0) {
		(void)setgrent();
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
