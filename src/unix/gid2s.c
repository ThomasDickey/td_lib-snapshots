/*
 * Title:	gid2s.c (gid/string converter)
 * Author:	T.E.Dickey
 * Created:	10 Nov 1987
 * Modified:
 *		03 Sep 1995, use btree rather than linked list, to speed up
 *			     sorting in 'ded'.
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

#define	GRP_PTYPES
#define	STR_PTYPES
#include <td_btree.h>

MODULE_ID("$Id: gid2s.c,v 12.10 2000/06/30 10:46:01 tom Exp $")

#if HAVE_GETGRGID

typedef	struct {
	int		user;
	char		*name;
	} GID_DATA;

static char * lookup_gid (
	_AR1(int,	gid))
	_DCL(int,	gid)
{
	register struct group *p;

	/* if not found, lookup/translate it for future use */
#ifdef	apollo
	if (gid == -3) {
		return txtalloc("<none>");
	} else
#endif
	if ((p = getgrgid(gid)) != 0)
		return txtalloc(p->gr_name);
	else {
		auto	char	bfr[80];
		(void)l2str(bfr, (long)gid, 0);
		return txtalloc(bfr);
	}
}

static
BI_NODE	*new_node (
	_AR1(void *,	data))
	_DCL(void *,	data)
{
	GID_DATA *value = (GID_DATA *)data;
	BI_NODE *result = BI_NODE_ALLOC(sizeof(GID_DATA));
	memset(result, 0, sizeof(*result));
	value->name = lookup_gid(value->user);
	memcpy((char *)&(result->value), (char *)data, sizeof(GID_DATA));
	return result;
}

static
int	cmp_node (
	_ARX(void *,	a)
	_AR1(void *,	b)
		)
	_DCL(void *,	a)
	_DCL(void *,	b)
{
	return ((GID_DATA *)a)->user
	   -   ((GID_DATA *)b)->user;
}

static
void	dpy_node (
	_AR1(void *,	a))
	_DCL(void *,	a)
{
	PRINTF("%d:%s",
		((GID_DATA *)a)->user,
		((GID_DATA *)a)->name);
}

static	BI_TREE	gid2s_tree = {
	cmp_node,
	new_node,
	dpy_node,
	BI_NODE_NULL
	};

char *	gid2s(
	_AR1(int,	user))
	_DCL(int,	user)
{
	static GID_DATA data;
	data.user = user;
	memcpy((char *)&data, (char *)btree_find(&gid2s_tree, &data), sizeof(GID_DATA));
	return data.name;
}

#ifdef	TEST
_MAIN
{
	register int	j;
	auto	 char	*d;
	auto	 int	user;

	PRINTF("argc:%d\n", argc);
	if (argc > 1) {
		for (j = 1; j < argc; j++) {
			user = strtol(argv[j], &d, 0);
			if (*d) {
				PRINTF("? illegal character /%s/\n", d);
				continue;
			}
			PRINTF("%d => \"%s\"\n", user, gid2s(user));
		}
	} else {
		int	tst_len;
		char	**tst_vec;
		tst_len = file2argv("/etc/group", &tst_vec);
		PRINTF("tst_len:%d\n", tst_len);
		for (j = 0; j < tst_len; j++) {
			if ((d = strchr(tst_vec[j], ':')) == 0)
				continue;
			*d++ = EOS;
			if ((d = strchr(d, ':')) == 0)
				continue;
			user = atoi(++d);
			PRINTF("%s -> %d -> %s\n", tst_vec[j], user, d = gid2s(user));
			if (strcmp(tst_vec[j], d))
				PRINTF("** DIFF\n");
		}
		btree_dump(&gid2s_tree);
	}
	exit(SUCCESS);
}
#endif	/* TEST */

#endif	/* HAVE_GETGRGID */
