#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: gid2s.c,v 12.3 1993/11/26 22:03:58 dickey Exp $";
#endif

/*
 * Title:	gid2s.c (gid/string converter)
 * Author:	T.E.Dickey
 * Created:	10 Nov 1987
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		04 Oct 1989, speedup (?) by reading ids only as needed
 *		25 Jul 1989, recompiled with apollo SR10 -- mods for function
 *			     prototypes
 *		28 Jul 1988, added "<none>" for apollo.
 *
 * Function:	Maintain a lookup table of gid names for fast access.
 *		For any given argument, return a pointer to string
 *		defining the name.
 */

#define	STR_PTYPES
#include	"ptypes.h"

#ifdef	unix
#include	<grp.h>

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
void	define_gid2s(
	_ARX(int,	gid)
	_AR1(char *,	name)
		)
	_DCL(int,	gid)
	_DCL(char *,	name)
{
	register TABLE	*q = ALLOC(TABLE,1);
	q->link  = table_gid2s;
	q->group = gid;
	q->name  = txtalloc(name);
	table_gid2s = q;
}

char *
gid2s(
_AR1(int,	gid))
_DCL(int,	gid)
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
	if ((p = getgrgid(gid)) != 0)
		define_gid2s(gid, p->gr_name);
	else {
		auto	char	bfr[80];
		(void)ltostr(bfr, (long)gid, 0);
		define_gid2s(gid, bfr);
	}
	return (gid2s(gid));
}

#ifdef	TEST
_MAIN
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
#endif	/* TEST */
#endif	/* unix */
