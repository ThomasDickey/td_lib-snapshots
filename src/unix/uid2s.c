#ifndef	lint
static	char	Id[] = "$Id: uid2s.c,v 9.0 1991/05/15 09:56:54 ste_cm Rel $";
#endif

/*
 * Title:	uid2s.c (uid/string converter)
 * Author:	T.E.Dickey
 * Created:	10 Nov 1987
 * $Log: uid2s.c,v $
 * Revision 9.0  1991/05/15 09:56:54  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  09:56:54  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  90/05/15  08:14:46  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.1  90/05/15  08:14:46  dickey
 *		added a hack to read the apollo passwd-file directly to bypass
 *		a bug in their handling of obsolete accounts
 *		
 *		Revision 7.0  89/10/04  11:38:17  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
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
#endif

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

#ifdef	apollo_sr10
	int	len_passwd;	/* share with 's2uid.c' */
	char	**vec_passwd;
static	find_uid(bfr)
	char	*bfr;
{
	register int	j;
	register char	*s, *t;
	auto	 size_t	len = strlen(bfr);

	if (!len_passwd)
		len_passwd = file2argv("/etc/passwd", &vec_passwd);
	for (j = 0; j < len_passwd; j++) {
		if (s = strchr(vec_passwd[j], ':')) {
			if (t = strchr(s+1, ':')) {
				if (t[len+1] == ':'
				&& !strncmp(t+1, bfr, len)) {
					len = s - vec_passwd[j];
					(void)strncpy(bfr, vec_passwd[j], len);
					bfr[len] = EOS;
					return;
				}
			}
		}
	}
}
#define	unknown_uid	find_uid(bfr);
#else
#define	unknown_uid
#endif

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
		unknown_uid	/* try to recover! */
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
#endif
