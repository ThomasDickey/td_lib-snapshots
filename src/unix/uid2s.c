/*
 * Title:	uid2s.c (uid/string converter)
 * Author:	T.E.Dickey
 * Created:	10 Nov 1987
 * Modified:
 *		29 Nov 2019, gcc warnings
 *		05 Jul 2010, gcc warnings
 *		07 Mar 2004, remove K&R support, indent'd.
 *		03 Sep 1995, use btree rather than linked list, to speed up
 *			     sorting in 'ded'.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		15 May 1990, added a hack to read the apollo passwd-file
 *			     directly to bypass a bug in their handling of
 *			     obsolete accounts
 *		04 Oct 1989, speedup (?) by reading ids only as needed
 *		25 Jul 1989, recompiled with apollo SR10 -- mods for function
 *			     prototypes
 *
 * Function:	Maintain a lookup table of uid names for fast access.
 *		For any given argument, return a pointer to string
 *		defining the name.
 */

#define	PWD_PTYPES
#define	STR_PTYPES
#include <td_btree.h>

MODULE_ID("$Id: uid2s.c,v 12.20 2025/01/07 00:14:42 tom Exp $")

#if defined(HAVE_GETPWUID)

typedef struct {
    uid_t user;
    char *name;
} UID_DATA;

#ifdef	apollo_sr10
int len_passwd;			/* share with 's2uid.c' */
char **vec_passwd;

static void
find_uid(char *bfr)
{
    int j;
    char *s, *t;
    size_t len = strlen(bfr);

    if (!len_passwd)
	len_passwd = file2argv("/etc/passwd", &vec_passwd);
    for (j = 0; j < len_passwd; j++) {
	if ((s = strchr(vec_passwd[j], ':')) != 0) {
	    if ((t = strchr(s + 1, ':')) != 0
		&& t[len + 1] == ':'
		&& !strncmp(t + 1, bfr, len)) {
		len = s - vec_passwd[j];
		(void) strncpy(bfr, vec_passwd[j], len);
		bfr[len] = EOS;
		return;
	    }
	}
    }
}
#define	unknown_uid()	find_uid(bfr);
#else
#define	unknown_uid()
#endif

static char *
lookup_uid(uid_t uid)
{
    struct passwd *p;

    /* if not found, lookup/translate it for future use */
#ifdef	apollo
    if (uid == -3) {
	return txtalloc("<none>");
    } else
#endif
    if ((p = getpwuid(uid)) != NULL)
	return txtalloc(p->pw_name);
    else {
	char bfr[80];
	(void) l2str(bfr, (long) uid, 0);
	unknown_uid();		/* try to recover! */
	return txtalloc(bfr);
    }
}

static BI_NODE *
new_node(const void *data)
{
    UID_DATA value = *(const UID_DATA *) data;
    BI_NODE *result = BI_NODE_ALLOC(sizeof(UID_DATA));
    memset(result, 0, sizeof(*result));
    value.name = lookup_uid(value.user);
    memcpy(&(result->value), &value, sizeof(UID_DATA));
    return result;
}

static int
cmp_node(const void *a, const void *b)
{
    return ((int) ((const UID_DATA *) a)->user
	    - (int) ((const UID_DATA *) b)->user);
}

static void
dpy_node(const void *a)
{
    const UID_DATA *p = (const UID_DATA *) a;
    PRINTF("%d:%s",
	   (int) p->user,
	   p->name);
}

static BI_TREE uid2s_tree =
{
    cmp_node,
    new_node,
    dpy_node,
    BI_NODE_NULL
};

char *
uid2s(uid_t user)
{
    static UID_DATA data;
    data.user = user;
    memcpy(&data, btree_find(&uid2s_tree, &data), sizeof(UID_DATA));
    return data.name;
}

#ifdef	TEST
_MAIN
{
    int j;
    char *d;
    int user;

    PRINTF("argc:%d\n", argc);
    if (argc > 1) {
	for (j = 1; j < argc; j++) {
	    user = strtol(argv[j], &d, 0);
	    if (*d) {
		PRINTF("? illegal character /%s/\n", d);
		continue;
	    }
	    PRINTF("%d => \"%s\"\n", user, uid2s((uid_t) user));
	}
    } else {
	int tst_len;
	char **tst_vec;
	tst_len = file2argv("/etc/passwd", &tst_vec);
	PRINTF("tst_len:%d\n", tst_len);
	for (j = 0; j < tst_len; j++) {
	    if ((d = strchr(tst_vec[j], ':')) == 0)
		continue;
	    *d++ = EOS;
	    if ((d = strchr(d, ':')) == 0)
		continue;
	    user = atoi(++d);
	    PRINTF("%s -> %d -> %s\n", tst_vec[j], user, d = uid2s((uid_t) user));
	    if (strcmp(tst_vec[j], d))
		PRINTF("** DIFF\n");
	}
	btree_dump(&uid2s_tree);
    }
    exit(SUCCESS);
}
#endif /* TEST */

#endif /* HAVE_GETPWUID */
