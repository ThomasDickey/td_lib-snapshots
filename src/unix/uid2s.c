#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: uid2s.c,v 12.2 1993/10/29 17:35:23 dickey Exp $";
#endif

/*
 * Title:	uid2s.c (uid/string converter)
 * Author:	T.E.Dickey
 * Created:	10 Nov 1987
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		15 May 1990, added a hack to read the apollo passwd-file
 *			     directly to bypass a bug in their handling of
 *			     obsolete accounts
 *		04 Oct 1989, speedup (?) by reading ids only as needed
 *		
 *		25 Jul 1989, recompiled with apollo SR10 -- mods for function
 *			     prototypes
 *		
 * Function:	Maintain a lookup table of uid names for fast access.
 *		For any given argument, return a pointer to string
 *		defining the name.
 */

#define	PWD_PTYPES
#define	STR_PTYPES
#include	"ptypes.h"

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
void	define_uid2s(
	_ARX(int,	id)
	_AR1(char *,	name)
		)
	_DCL(int,	id)
	_DCL(char *,	name)
{
	register TABLE	*q = ALLOC(TABLE,1);
	q->link = table_uid2s;
	q->user = id;
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
	if ((p = getpwuid(uid)) != NULL)
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
_MAIN
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
