#ifndef	lint
static	char	Id[] = "$Id: gid2s.c,v 9.0 1991/05/15 09:26:18 ste_cm Rel $";
#endif

/*
 * Title:	gid2s.c (gid/string converter)
 * Author:	T.E.Dickey
 * Created:	10 Nov 1987
 * $Log: gid2s.c,v $
 * Revision 9.0  1991/05/15 09:26:18  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:26:18  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  89/10/04  11:37:50  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  89/10/04  11:37:50  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/10/04  11:37:50  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/10/04  11:37:50  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.1  89/10/04  11:37:50  dickey
 *		speedup (?) by reading ids only as needed
 *		
 *		Revision 4.0  89/07/25  09:03:38  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.1  89/07/25  09:03:38  dickey
 *		recompiled with apollo SR10 -- mods for function prototypes
 *		
 *		Revision 3.0  88/08/12  09:36:30  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
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

#define	STR_PTYPES
#include	"ptypes.h"
#include	<grp.h>
extern	char	*ltostr(),
		*txtalloc();

#ifdef	SYSTEM5
extern	 struct group  *getgrent();		/* cf: apollo sys5 */
extern		V_OR_I	setgrent();
extern		V_OR_I	endgrent();
#endif

typedef	struct	_table	{
	struct	_table	*link;
	int	group;
	char	*name;
	} TABLE;

#define	def_alloc	def_GID_TABLE	/* lint (gould) */
	/*ARGSUSED*/
	def_ALLOC(TABLE)

static	TABLE	*table_gid2s;

static
define_gid2s(gid, name)
int	gid;
char	*name;
{
	register TABLE	*q = ALLOC(TABLE,1);
	q->link  = table_gid2s;
	q->group = gid;
	q->name  = txtalloc(name);
	table_gid2s = q;
}

char *
gid2s(gid)
int	gid;
{
	register struct group *p;
	register TABLE	*q;

	/* search the table for previously-known items */
	for (q = table_gid2s; q; q = q->link)
		if (q->group == gid)
			return(q->name);

	/* if not found, lookup/translate it for future use */
#ifdef	apollo
	if (gid == -3) {
		define_gid2s(gid, "<none>");
	} else
#endif
	if (p = getgrgid(gid))
		define_gid2s(gid, p->gr_name);
	else {
		auto	char	bfr[80];
		(void)ltostr(bfr, (long)gid, 0);
		define_gid2s(gid, bfr);
	}
	return (gid2s(gid));
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
	register int	j;
	auto	 char	*d;
	auto	 int	group;

	for (j = 1; j < argc; j++) {
		group = strtol(argv[j], &d, 0);
		if (*d) {
			printf("? illegal character /%s/\n", d);
			continue;
		}
		printf("%d => \"%s\"\n", group, gid2s(group));
	}
	exit(SUCCESS);
}
#endif
