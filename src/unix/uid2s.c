#ifndef	lint
static	char	Id[] = "$Id: uid2s.c,v 7.0 1989/10/04 11:38:17 ste_cm Rel $";
#endif	lint

/*
 * Title:	uid2s.c (uid/string converter)
 * Author:	T.E.Dickey
 * Created:	10 Nov 1987
 * $Log: uid2s.c,v $
 * Revision 7.0  1989/10/04 11:38:17  ste_cm
 * BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *
 *		Revision 6.0  89/10/04  11:38:17  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/10/04  11:38:17  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.1  89/10/04  11:38:17  dickey
 *		speedup (?) by reading ids only as needed
 *		
 *		Revision 4.0  89/07/25  09:29:38  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.1  89/07/25  09:29:38  dickey
 *		recompiled with apollo SR10 -- mods for function prototypes
 *		
 *		Revision 3.0  88/08/12  09:36:56  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/12  09:36:56  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.10  88/08/12  09:36:56  dickey
 *		sccs2rcs keywords
 *		
 * Function:	Maintain a lookup table of uid names for fast access.
 *		For any given argument, return a pointer to string
 *		defining the name.
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	<pwd.h>
extern	char	*ltostr(),
		*txtalloc();

#ifdef	SYSTEM5
extern	 struct passwd *getpwent();
extern		V_OR_I	setpwent();
extern		V_OR_I	endpwent();
#endif	SYSTEM5

typedef	struct	_table	{
	struct	_table	*link;
	int		user;
	char		*name;
	} TABLE;

#define	def_alloc	def_UID_TABLE	/* lint (gould) */
	/*ARGSUSED*/
	def_ALLOC(TABLE)

static	TABLE	*table_uid2s;

static
define_uid2s(uid, name)
int	uid;
char	*name;
{
	register TABLE	*q = ALLOC(TABLE,1);
	q->link = table_uid2s;
	q->user = uid;
	q->name = txtalloc(name);
	table_uid2s = q;
}

char *
uid2s(uid)
int	uid;
{
	register struct passwd *p;
	register TABLE	*q;

	/* search the table for previously-known items */
	for (q = table_uid2s; q; q = q->link)
		if (q->user == uid)
			return(q->name);

	/* if not found, lookup/translate it for future use */
#ifdef	apollo
	if (uid == -3) {
		define_uid2s(uid, "<none>");
	} else
#endif
	if (p = getpwuid(uid))
		define_uid2s(uid, p->pw_name);
	else {
		auto	char	bfr[80];
		(void)ltostr(bfr, (long)uid, 0);
		define_uid2s(uid, bfr);
	}
	return (uid2s(uid));
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
	register int	j;
	auto	 char	*d;
	auto	 int	user;

	for (j = 1; j < argc; j++) {
		user = strtol(argv[j], &d, 0);
		if (*d) {
			printf("? illegal character /%s/\n", d);
			continue;
		}
		printf("%d => \"%s\"\n", user, uid2s(user));
	}
	exit(SUCCESS);
}
#endif	TEST
